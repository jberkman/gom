(function () {
    print (dom = Components.classes["@ilovegom.org/dom-implementation;1"].createInstance(Components.interfaces.nsIDOMDOMImplementation));
    print (dom.hasFeature ("xml", "1.0"));
})();
