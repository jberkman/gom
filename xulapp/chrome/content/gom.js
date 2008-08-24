function main () {
    const Ci = Components.interfaces;
    dump ("Ok, let's see what we've got.\n");
    var cmdLine = window.arguments[0];
    cmdLine = cmdLine.QueryInterface (Ci.nsICommandLine);
    var url = cmdLine.getArgument (0);
    alert (url);
};
function go () {
    var urlbar = document.getElementById ('urlbar');
    var browser = document.getElementById ('browser');
    browser.loadURI (urlbar.value, null, null);
};
//addEventListener("load", main, false);
