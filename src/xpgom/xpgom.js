(function () {
    print (dom = Components.classes["@ilovegom.org/dom-implementation;1"].createInstance(Components.interfaces.nsIDOMDOMImplementation));
    print (dom.hasFeature ("xml", "1.0"));
    print (doc = dom.createDocument (null, "gom", null));
    print (doc.nodeType);
    print (doc.nodeName);
})();
