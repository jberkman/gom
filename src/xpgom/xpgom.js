/*
The MIT License

Copyright (c) 2008 jacob berkman <jacob@ilovegom.org>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
 */
//(function (args) {
var args = arguments;
if (args.length == 0) {
    print ('Usage: xpgom.js <url>');
    quit (1);
}

const Cc = Components.classes;
const Ci = Components.interfaces;
var io = Cc['@mozilla.org/network/io-service;1'].getService (Ci.nsIIOService);

var gom = Cc['@ilovegom.org/shell;1'].createInstance (Ci.nsIDocShell);
var uri = io.newURI (args[0], null, null);

gom.setCurrentURI (uri);

//gom.dump ('userAgent: ' + gom.navigator.userAgent);
//gom.dump ('Location: ' + gom.location);
//gom.dump ('XHR: ' + gom.XMLHttpRequest);
//gom.dump (gom.document.documentURI);
//gom.dump (gom.document.documentElement.tagName);

//gom.run ();

//})(arguments);
