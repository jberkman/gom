//(function (args) {
var args = arguments;
if (args.length == 0) {
    print ('Usage: xpgom.js <url>');
    quit (1);
}

var Cc = Components.classes;
var Ci = Components.interfaces;

var gom = Cc['@ilovegom.org/context;1'].createInstance (Ci.xgIGomContext);
gom.init (args[0]);

var serializer = Cc['@mozilla.org/xmlextras/xmlserializer;1'].createInstance(Ci.nsIDOMSerializer);
print (serializer.serializeToString (gom.document));

gom.run ();

//})(arguments);
