var xgGomParserModule = { };
function NSGetModule (aCompMgr, aFileSpec) { return xgGomParserModule; }

(function () {

var Cc = Components.classes;
var Ci = Components.interfaces;

const CLASS_ID = Components.ID("{E643FF76-1F9C-452A-A0A6-421B1AA1AFE5}");
const CLASS_NAME = "An XML Parser for Gom";
const CONTRACT_ID = "@ilovegom.org/parser;1";

function xgGomParser() {
    this.wrappedJSObject = this;
};

function getDOMImplementation() {
    // return  Cc['@ilovegom.org/dom-implementation;1'].createInstance(Ci.nsIDOMDOMImplementation);
    var parser = Cc['@mozilla.org/xmlextras/domparser;1'].createInstance (Ci.nsIDOMParser);
    var document = parser.parseFromString('', "text/xml");
    return document.implementation;
};

xgGomParser.prototype = {
    // xgIGomParser
    parseURI: function (uri, listener) {
	var io = Cc['@mozilla.org/network/io-service;1'].getService (Ci.nsIIOService);
	var uri = io.newURI (uri, null, null);

	var chan = io.newChannelFromURI (uri);
	var stream = chan.open();

	var reader = Cc['@mozilla.org/saxparser/xmlreader;1'].createInstance (Ci.nsISAXXMLReader);
	var document = null;
	reader.contentHandler = {
	    elem: null,

	    // nsISAXContentHandler
	    startDocument: function () { },
	    endDocument: function () { },

	    startElement: function (nsUri, localName, qName, /*nsISAXAttributes*/ attributes) {
		var elem;
		if (document) {
		    elem = document.createElementNS (nsUri, qName);
		} else {
		    document = getDOMImplementation ().createDocument (nsUri, qName, null);
		    if (listener) {
			listener.documentCreated (document);
		    }
		    elem = document.documentElement;
		}
		for(var i = 0; i < attributes.length; i++) {
		    elem.setAttribute (attributes.getLocalName (i),
		    attributes.getValue (i));
		}
		this.elem = this.elem ? this.elem.appendChild (elem) : elem;
	    },

	    endElement: function(uri, localName, qName) {
		this.elem = this.elem.parentNode;
	    },

	    characters: function(value) { },
	    processingInstruction: function(target, data) { },
	    ignorableWhitespace: function(whitespace) { },
	    startPrefixMapping: function(prefix, uri) { },
	    endPrefixMapping: function(prefix) { },

	    // nsISupports
	    QueryInterface: function(iid) {
		if(!iid.equals(Ci.nsISupports) &&
		   !iid.equals(Ci.nsISAXContentHandler))
		    throw Components.results.NS_ERROR_NO_INTERFACE;
		return this;
	    }
	};
	reader.parseFromStream (stream, 'UTF-8', 'text/xml');
	return document;
    },
    // nsISupports
    QueryInterface: function (aIID) {
	if (!aIID.equals(Ci.xgIGomParser) &&
	    !aIID.equals(Ci.nsISupports))
	    throw Components.results.NS_ERROR_NO_INTERFACE;
	return this;
    }
};

var xgGomParserFactory = {
    createInstance: function (aOuter, aIID) {
	if (aOuter != null) {
	    throw Components.results.NS_ERROR_NO_AGGREGATION;
	}
	return (new xgGomParser).QueryInterface (aIID);
    }
};

xgGomParserModule.registerSelf = function(aCompMgr, aFileSpec, aLocation, aType)
{
    aCompMgr = aCompMgr.QueryInterface (Components.interfaces.nsIComponentRegistrar);
    aCompMgr.registerFactoryLocation (CLASS_ID, CLASS_NAME, CONTRACT_ID,
				      aFileSpec, aLocation, aType);
};

xgGomParserModule.unregisterSelf = function (aCompMgr, aLocation, aType)
{
    aCompMgr = aCompMgr.QueryInterface (Components.interfaces.nsIComponentRegistrar);
    aCompMgr.unregisterFactoryLocation(CLASS_ID, aLocation);
};

xgGomParserModule.getClassObject = function(aCompMgr, aCID, aIID)
{
    if (!aIID.equals(Ci.nsIFactory)) {
	throw Components.results.NS_ERROR_NOT_IMPLEMENTED;
    }
    if (aCID.equals(CLASS_ID))
	return xgGomParserFactory;

    throw Components.results.NS_ERROR_NO_INTERFACE;
};

xgGomParserModule.canUnload = function(aCompMgr) { return true; }

})();
