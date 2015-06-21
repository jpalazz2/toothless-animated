var initialized = false;
var key_battery = 0;
var key_date_format = 1;
var key_bluetooth = 2;
var options = {};

function fixedEncodeURIComponent(str)
{
	return encodeURIComponent(str).replace(/[!'()]/g, escape).replace(/\*/g, "%2A");
}

function buildURL()
{
	var url = 'http://josephkpalazzolo.com/pebblefaces/toothless_configurable_june2015.html?';
	if (options['battery-indicator'])
		url += fixedEncodeURIComponent("battery-indicator") + '=' + fixedEncodeURIComponent(options['battery-indicator']) + '&';
	if (options['date-format'])
		url +=fixedEncodeURIComponent("date-format") + '=' + fixedEncodeURIComponent(options['date-format']);
	if (options['bluetooth-indicator'])
		url +=fixedEncodeURIComponent("bluetooth-indicator") + '=' + fixedEncodeURIComponent(options['bluetooth-indicator']);
	return url;
}

Pebble.addEventListener("ready", function() {
  	console.log("ready called!");
  	initialized = true;
});

Pebble.addEventListener("showConfiguration", function() {
  	console.log("showing configuration");
	var battery = localStorage.getItem(key_battery);
	var date_format = localStorage.getItem(key_date_format);
	var bluetooth_indicator = localStorage.getItem(key_bluetooth);
	options['battery-indicator'] = battery;
	options['date-format'] = date_format;
	options['bluetooth-indicator'] = bluetooth_indicator;
	var url = buildURL();
  	Pebble.openURL(url);
});

Pebble.addEventListener("webviewclosed", function(e) {
  	console.log("configuration closed");
  	// webview closed
  	//Using primitive JSON validity and non-empty check
	options = JSON.parse(decodeURIComponent(e.response));
  	if (options['battery-indicator'] && options['date-format'] && e.response.length > 5) {
		var battery;
		if (options['battery-indicator'] == 'on')
			battery = 1;
		else
			battery = 0;
		var dateConfig;
		if (options['date-format'] == 'a')
			{
				dateConfig = 1;
				console.log("Date format a");
			}
		else
			dateConfig = 0;
		var bluetooth_indicator;
		if (options['bluetooth-indicator'] == 'on') {
			bluetooth-indicator = 1;
		} else {
			bluetooth-indicator = 0;
		}
		var dictionary = {
			"KEY_BATTERY": battery,
			"KEY_DATE_FORMAT": dateConfig,
			"KEY_BLUETOOTH": bluetooth_indicator;
		};
		Pebble.sendAppMessage(dictionary,
							 function(e) {
								 console.log("Sent app message");
							 },
							 function(e) {
								 console.log("Failed to send app message");
							 });
    	console.log("Options = " + JSON.stringify(options));
		localStorage.setItem(key_battery, options['battery-indicator']);
		localStorage.setItem(key_date_format, options['date-format']);
		localStorage.setItem(key_bluetooth, options['bluetooth-indicator']);
  	} else {
    	console.log("Canceled");
  	}
});
