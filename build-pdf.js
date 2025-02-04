const fs = require("fs");
const path = require("path");
const puppeteer = require("puppeteer");
const markdown = require("markdown-it")({
    html: true, // Enable HTML tags in source
    linkify: true, // Autoconvert URL-like text to links
    typographer: true // Enable smartypants and other sweet transforms
});

// Directory paths
const lessonsDir = './lessons';
const outputDir = './lessons-pdf';

// Ensure the output directory exists
if (!fs.existsSync(outputDir)) {
    fs.mkdirSync(outputDir);
}

// Read all folders in the lessons directory
fs.readdir(lessonsDir, async (err, folders) => {
    if (err) {
        console.error("Error reading lessons directory:", err);
        process.exit(1);
    }

    const browser = await puppeteer.launch();
    const page = await browser.newPage();

    for (const folder of folders) {
        const folderPath = path.join(lessonsDir, folder);
        const readmePath = path.join(folderPath, 'README.md');
        const outputFilePath = path.join(outputDir, `${folder}.pdf`);

        // Check if README.md exists in the folder
        if (fs.existsSync(readmePath)) {
            let markdownContent = fs.readFileSync(readmePath, 'utf-8');

            // Adjust image paths to use absolute paths without file:// protocol
            markdownContent = markdownContent.replace(/!\[([^\]]*)\]\((\.\/[^\)]+)\)/g, (match, alt, src) => {
                const absolutePath = path.resolve(folderPath, src);
                return `![${alt}](${absolutePath})`;
            });

            const htmlContent = markdown.render(markdownContent);

            // Add CSS styles for code blocks and blockquotes
            const styledHtmlContent = `
                <style>
                    pre {
                        background: #f5f5f5;
                        border: 1px solid #ddd;
                        padding: 10px;
                        border-radius: 4px;
                    }
                    blockquote {
                        border-left: 5px solid #ccc;
                        padding-left: 10px;
                        margin-left: 0;
                        color: #555;
                    }
                </style>
                ${htmlContent}
            `;

            // Set the base URL to the folder path to resolve relative paths
            await page.goto(`file://${path.resolve(folderPath)}`, { waitUntil: 'networkidle0' });
            await page.setContent(styledHtmlContent, { waitUntil: 'networkidle0' });
            await page.pdf({
                path: outputFilePath,
                format: 'A4',
                margin: {
                    top: '20mm',
                    right: '20mm',
                    bottom: '20mm',
                    left: '20mm'
                }
            });

            console.log("PDF generated:", outputFilePath);
        } else {
            console.warn(`README.md not found in ${folderPath}`);
        }
    }

    await browser.close();
});
