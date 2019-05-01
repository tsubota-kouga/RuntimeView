
const express = require("express");
const attach = require('neovim').attach;
const marked = require('marked');
const hljs = require('highlight');
// const fs = require('fs');
// const exec = require('child_process').exec;
// const latex = require('latex.js');
let socket = '';
let lang = '';
let html = '';
let file_change = 'true';
let nvim;
let buf = undefined;
let app = express();

app.get("/", function (req, res) {
    socket = req.query.socket || '';
    lang = req.query.lang || '';
    file_change = req.query.file_change || 'false';
    (async function(){
        // TODO not to change buf even if current buffer changed
        if(file_change == 'true'){ buf = undefined; }
        if(buf == undefined || file_change == 'true'){
            // change buffer handler
            nvim = attach({ socket: socket });
            buf = await nvim.buffer;
        }
        const lines = await buf.lines;
        if(lang == 'markdown') {
            const css_name = await nvim.eval('g:runtime_view#css_name');

            html = marked(lines.join('\n'), 'highlight');
            html = '<link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/highlight.js/9.12.0/styles/' + css_name + '.min.css">' +
                '<script src="https://cdnjs.cloudflare.com/ajax/libs/highlight.js/9.12.0/highlight.min.js"></script>' +
                '<script>hljs.initHighlightingOnLoad();</script>' + html;

        }
        else if(lang == 'html' || lang == 'xhtml')
        {
            html = lines.join('\n');
        }
        else if(lang == 'php')
        {
            // const file = 'runtime.php';
            // fs.writeFileSync(file, lines.join('\n'), 'utf-8');
            // exec('php runtime.php', (err, stdout, stderr) => {
            //     if(err !== null){
            //         html = '<strong>EXEC ERROR</strong>';
            //         return;
            //     }
            //     else
            //     {
            //         html = stdout;
            //     }
            // });
        }
        else if(lang == 'tex')
        {
            html = '<head><script type="text/javascript" src="http://cdn.mathjax.org/mathjax/latest/MathJax.js?config=TeX-AMS-MML_HTMLorMML"></script><script type="text/x-mathjax-config">MathJax.Hub.Config({ TeX: { equationNumbers: { autoNumber: "AMS" }}, tex2jax: { inlineMath: [ ["$","$"], ["\\(","\\)"] ], processEscapes: true }, "HTML-CSS": { matchFontHeight: false }, displayAlign: "left", displayIndent: "2em" });</script></head><body>';
            html += lines.join('\n');
            html += '</body>';
            // let generator = new latex.HtmlGenerator({hyphenate: false});
            // let doc = latex.parse(lines.join('\n'), { generator: generator }).htmlDocument();
            // html = doc.outerHTML;
        }
        res.status(200).send(html);
    })();
});

if(process.argv.listen < 3)
{
    console.log("cmd argment is required: port number");
}
else
{
    app.listen(Number(process.argv[2]), 'localhost');
}
