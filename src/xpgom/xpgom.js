//(function (args) {
args = arguments;
    if (args.length == 0) {
        print ('Usage: xpgom.js <url>');
        quit (1);
    }
    var Cc = Components.classes;
    var Ci = Components.interfaces;
    var req = Cc['@mozilla.org/xmlextras/xmlhttprequest;1'].createInstance(Ci.nsIXMLHttpRequest);
    print (req);
    print ('Opening: ' + args[0]);
    req.open ('GET', args[0], false);
    req.send(null);
    print (req.responseXML);

    var serializer = Cc['@mozilla.org/xmlextras/xmlserializer;1'].createInstance(Ci.nsIDOMSerializer);
    if (0) {
        print (serializer.serializeToString(req.responseXML));
    }

    var root = req.responseXML.documentElement;

    var dom = Cc['@ilovegom.org/dom-implementation;1'].createInstance(Ci.nsIDOMDOMImplementation);
    print (dom);
    print ('has xml: ' + dom.hasFeature ('xml', '1.0'));
    
    var doc = dom.createDocument (root.namespaceURI, root.nodeName, null);
    print (doc);
    if (0) {
        print (serializer.serializeToString (doc));
    }

    var gomRoot = doc.importNode (root, true);
    print (gomRoot);
    print (gomRoot.nodeType);
    print (gomRoot.nodeName);

    doc.replaceChild (gomRoot, doc.documentElement);
    print (doc.documentElement.nodeName);
//})(arguments);
