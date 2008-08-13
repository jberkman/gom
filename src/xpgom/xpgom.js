//(function (args) {
args = arguments;
if (args.length == 0) {
    print ('Usage: xpgom.js <url>');
    quit (1);
}

var Cc = Components.classes;
var Ci = Components.interfaces;

var io = Cc['@mozilla.org/network/io-service;1'].getService (Ci.nsIIOService);
var uri;
try {
    uri = io.newURI (args[0], null, null);
} catch (e) {
    print ('could not create uri: ' + e);
    quit (1);
}

var chan = io.newChannelFromURI (uri);
var stream = chan.open();

var reader = Cc['@mozilla.org/saxparser/xmlreader;1'].createInstance (Ci.nsISAXXMLReader);
var doc = null;
reader.contentHandler = {
    elem: null,

    // nsISAXContentHandler
    startDocument: function () {
	print ("startDocument");
    },

    endDocument: function () {
        print ("endDocument");
    },

    startElement: function (nsUri, localName, qName, /*nsISAXAttributes*/ attributes) {
	var elem;
	if (doc) {
	    elem = doc.createElementNS (nsUri, qName);
	} else {
	    var dom = Cc['@ilovegom.org/dom-implementation;1'].createInstance(Ci.nsIDOMDOMImplementation);
	    doc = dom.createDocument (nsUri, qName, null);
	    elem = doc.documentElement;
	}
        for(var i = 0; i < attributes.length; i++) {
	    elem.setAttribute (attributes.getLocalName (i),
			       attributes.getValue (i));
        }
	this.elem = this.elem ? this.elem.appendChild (elem) : elem;
    },

    endElement: function(uri, localName, qName) {
	this.elem = this.elem.parentNode;
        print("endElement: namespace='" + uri + "', localName='" +
              localName + "', qName='" + qName + "'");
    },

    characters: function(value) {
        print("characters: " + value);
    },

    processingInstruction: function(target, data) {
        print("processingInstruction: target='" + target + "', data='" +
	      data + "'");
    },

    ignorableWhitespace: function(whitespace) {
        // don't care
    },

    startPrefixMapping: function(prefix, uri) {
        // don't care
    },

    endPrefixMapping: function(prefix) {
        // don't care
    },

    // nsISupports
    QueryInterface: function(iid) {
        if(!iid.equals(Ci.nsISupports) &&
           !iid.equals(Ci.nsISAXContentHandler))
            throw Components.results.NS_ERROR_NO_INTERFACE;
        return this;
    }
};
reader.parseFromStream (stream, 'UTF-8', 'text/xml');

var parser = Cc['@mozilla.org/xmlextras/domparser;1'].createInstance (Ci.nsIDOMParser);

var nsdoc = parser.parseFromString ('<?xml version="1.0"?>\n<a/>', 'text/xml')
nsdoc.replaceChild (nsdoc.documentElement, nsdoc.importNode (doc.documentElement, true));

var serializer = Cc['@mozilla.org/xmlextras/xmlserializer;1'].createInstance(Ci.nsIDOMSerializer);
print (serializer.serializeToString (nsdoc));

//})(arguments);
