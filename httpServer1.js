var http = require('http');
var fs = require('fs');
var url = require('url');
var path = require('path');



function process_get(req,res) {
    var contentTypesByExtension = {
        '.html': "text/html",
        '.css':  "text/css",
        '.js':   "text/javascript"
    };

    fs.exists(filename, function(exists){
        if(!exists) {
            res.writeHead(404,{"Content-Type":"text/plain"});
            res.write("404 Not Found\n");
            res.end();
            return;
        }
        if ( fs.statSync(filename).isDirectory() ) 
            filename += "/index.html";
        fs.readFile(filename, "binary", function(err, file){
            if ( err ) {
                res.writeHead(500, {"Content-Type":"text/plain"});
                res.write(err +"\n");
                res.end();
                return;
            }

            var headers = {};
            var contentType = contentTypesByExtension[path.extname(filename)];
            if (contentType) headers["Content-Type"] = contentType;
            res.writeHead(200, headers);
            res.write(file, "binary");
            res.end();
        });
    });
}


function create_http_server() 
{
    http.createServer(function(req, res) {
        var uri = url.parse(req.url).pathname;
        var filename = path.join(process.cwd()+"/www", uri);
        console.log("req.method " + req.method);
        console.log("uri " + uri);
        console.log("req.url " + req.url);

        if ( req.method == 'GET' ) {
            process_get(req,res);
        }
    }).listen(8001);
}

function main() {
    console.log("Http Server Example");
    create_http_server();
}

main();

