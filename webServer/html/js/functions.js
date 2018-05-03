<<<<<<< HEAD
    document.onreadystatechange = function() {
        if (document.readyState === 'complete') {
            initApp();
            //dom is ready, window.onload fires later
        }
    };


    var checkbox = document.getElementById('switch1');
    var wrapper = document.getElementById("input-fields");
    var plus = document.getElementById('plus');
    var inSSID = document.getElementById('ssid');
    var inPASS = document.getElementById('password');
    var bool = true;

    function initApp(){
        
        document.querySelectorAll("input").forEach(function(elem){ //add listener to all <input>
            elem.addEventListener('keypress',function(e){
                var key = e.which || e.keyCode;
                if (key === 13) { // 13 is enter
                  save();
                }
            });
        });
        
        var data = {
            "req": "network_status"
        };

        httpGetAsync('POST', 'switch.json', data, function(err, res) {
            if (err) {
                console.log('Received error');
            } else {
                console.log('Received +' + JSON.stringify(res)+'+');
                var ans =JSON.parse(JSON.stringify(res));

                if (new String(ans.status) == "online") {
                   
                } else {
                    wrapper.hidden = false;
                    checkbox.checked = false;
                }
            }
        });
        data.req = "check";
        httpGetAsync('POST', 'switch.json', data, function(err, res) {
            if (err) {
                console.log('Received error');
            } else {
                console.log('Received ~' + JSON.stringify(res)+'~');
                var ans =JSON.parse(JSON.stringify(res));

                if (new String(ans.status) == "true") {
                    checkbox.checked = true;
                } else {
                    checkbox.checked = false;
                }
            }
        });
        //checkbox.focus();
    }

    function save(){
        var boole = false;
        if(inSSID.value==""){
            inSSID.style.outline = "none";
            inSSID.style.boxShadow = "0 0 0 2px red";
            boole=true;
        }else{
            inSSID.style.boxShadow = "0 0 0 2px green";
        }

        console.log("save()");
        if(boole){return;}

        var data = {
            "req": "add",
            "ssid": inSSID.value,
            "pass": inPASS.value
        }
        console.log(data);
        httpGetAsync('POST', 'switch.json', data, function(err, res) {
            if (err) {
                console.log('Received error');
            } else {
                console.log('Received ~' + JSON.stringify(res)+'~');
            }
        });
    }

    function addNetworkConnection(){
        wrapper.hidden=!wrapper.hidden;
        plus.textContent = (plus.textContent=="+")?"-":"+";
    }

    function switchState() {
        var data = {
            "req": "trigger",
            "status": !checkbox.checked
        };
        httpGetAsync('POST', 'switch.json', data, function(err, res) {
            if (err) {
                console.log('Received error');
            } else {
                console.log('Received -' + JSON.stringify(res)+'-');
                var req = JSON.parse(JSON.stringify(res));
                console.log(res.status);
                if (new String(res.status) == "success") {
                    // console.log('Received ' + res);
                    checkbox.checked = !checkbox.checked;
                }else{
                    console.log('Error: ' + res.error);
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
=======
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
>>>>>>> 3eeefce814e5c6cfd3033fcbb88c66475ee768c6
