var checkbox = document.getElementById('switch1');

document.onreadystatechange = function(e) {
    var data = {
        "checkStatus": true
    };

    httpGetAsync('POST', 'switch.json', data, function(err, res) {
        if (err) {
            console.log('Received error');
        } else {
            // console.log('Received ~' + res+'~');
            if (new String(res) == "true") {
                checkbox.checked = true;
            } else {
                checkbox.checked = false;
            }
        }
    });
    checked.focus();
    if (document.readyState === 'complete') {
        //dom is ready, window.onload fires later
    }
};

function switchState() {
    var data = {
        "checkStatus": false,
        "command": !checkbox.checked
    };

    httpGetAsync('POST', 'switch.json', data, function(err, res) {
        if (err) {
            console.log('Received error');
        } else {
            if (new String(res) == "true") {
                // console.log('Received ' + res);
                checkbox.checked = !checkbox.checked;
            }
        }
    });
}


function httpGetAsync(method, theUrl, data, callback) {
    var xmlHttp = new XMLHttpRequest();
    xmlHttp.onreadystatechange = function() {
        // this === xmlHttp (use whatever suits you)
        if (this.readyState === 4) {
            var response = JSON.parse(this.responseText);
            var err = null;
            if (this.status === 200) {
                //success
            } else {
                err = new Error('Some random error');
            }
            callback(err, response);
        }
    };
    xmlHttp.open(method, theUrl, true); // true for asynchronous
    xmlHttp.setRequestHeader('Content-type', 'application/json');
    xmlHttp.send(JSON.stringify(data));
}