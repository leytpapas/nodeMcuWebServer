    document.onreadystatechange = function() {
        if (document.readyState === 'complete') {
            initApp();
            //dom is ready, window.onload fires later
        }
    };

    var checkbox = document.getElementById('switchin');
    var plus_ap = document.getElementById('plus-ap');
    var plus_net = document.getElementById('plus-net');
    var inSSID_ap = document.getElementById('ssid-ap');
    var inPASS_ap = document.getElementById('password-ap');
    var inSSID_net = document.getElementById('ssid-net');
    var inPASS_net = document.getElementById('password-net');

    function initApp(){
        
        document.querySelectorAll("input.net").forEach(function(elem){ //add listener to all <input class="net">
            elem.addEventListener('keypress',function(e){
                var key = e.which || e.keyCode;
                if (key === 13) { // 13 is enter
                  save_net();
                }
            });
        });
        document.querySelectorAll("input.ap").forEach(function(elem){ //add listener to all <input class="ap">
            elem.addEventListener('keypress',function(e){
                var key = e.which || e.keyCode;
                if (key === 13) { // 13 is enter
                  save_ap();
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

    function save_ap(){
        var boole = false;
        var message = ""
        if(inSSID_ap.value==""){
            inSSID_ap.style.outline = "none";
            inSSID_ap.style.boxShadow = "0 0 0 2px red";
            message += "SSID cannot be empty.";
            boole=true;
        }

        if(inPASS_ap.value.length!=0 && inPASS_ap.value.length<8){
            inPASS_ap.style.outline = "none";
            inPASS_ap.style.boxShadow = "0 0 0 2px red";
            message +=(message=="")?"":"\n";
            message += "Password can either have 0 or more than 8 characters.";
            boole=true;
        }

        console.log("save_ap()");
        if(boole){
            document.getElementById('error-ap').innerHTML = message;
            document.getElementById('error-ap').hidden = false;
            return;
        }
        inSSID_ap.style.boxShadow = "0 0 0 2px green";
        inPASS_ap.style.boxShadow = "0 0 0 2px green";
        document.getElementById('error-ap').hidden = true;

        var data = {
            "req": "apconf",
            "ssid": inSSID_ap.value,
            "pass": inPASS_ap.value
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

    function save_net(){
        var boole = false;
        if(inSSID_net.value==""){
            inSSID_net.style.outline = "none";
            inSSID_net.style.boxShadow = "0 0 0 2px red";
            document.getElementById('error-net').innerHTML = "SSID cannot be empty.";
            document.getElementById('error-net').hidden = false;
            boole=true;
        }else{
            inSSID_net.style.boxShadow = "0 0 0 2px green";
        }

        console.log("save_net()");
        if(boole){return;}
        document.getElementById('error-net').hidden = true;
        var data = {
            "req": "connect",
            "ssid": inSSID_net.value,
            "pass": inPASS_net.value
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

    function showPass(checkboxEl,passwordEl){
        document.getElementById(passwordEl).type =(checkboxEl.checked)?"text":"password";
    }
    function toggle(button,otherbutton,wrapper){
        document.getElementById(wrapper).hidden =! document.getElementById(wrapper).hidden;
        if(button.textContent=="+"){
            button.textContent = "-";
            document.getElementById(otherbutton).hidden = true;
        }else{
            button.textContent ="+";
            document.getElementById(otherbutton).hidden = false;
        }
        //button.textContent = (button.textContent=="+")?"-":"+";
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