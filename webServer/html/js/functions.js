    document.onreadystatechange = function() {
        if (document.readyState === 'complete') {
            initApp();
            //dom is ready, window.onload fires later
        }
    };

    //**Reload page/tab on focus** or Better get status of switch
    window.onfocus = function() {
        //window.location.reload(true);
        getSwitchStatus();
    };

    var checkbox = document.getElementById('switchin');
    var plus_ap = document.getElementById('plus-ap');
    var plus_net = document.getElementById('plus-net');
    var inSSID_ap = document.getElementById('ssid-ap');
    var inPASS_ap = document.getElementById('password-ap');
    var inSSID_net = document.getElementById('ssid-net');
    var inPASS_net = document.getElementById('password-net');

    /*
    Init = addListeners, get status of checkbox
    */
    function initApp(){
        //add listener to all <input class="net">
        document.querySelectorAll("input.net").forEach(function(elem){ 
            elem.addEventListener('keypress',function(e){
                var key = e.which || e.keyCode;
                if (key === 13) { // 13 is enter
                  save_net();
                }
            });
        });
        //add listener to all <input class="ap">
        document.querySelectorAll("input.ap").forEach(function(elem){ 
            elem.addEventListener('keypress',function(e){
                var key = e.which || e.keyCode;
                if (key === 13) { // 13 is enter
                  save_ap();
                }
            });
        });

        getSwitchStatus();
    }
    /*
    Get status of switch
    */
    function getSwitchStatus(){var data = {
            "req": "check"
        };
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
    }
    /*
    Submitting form for access point settings to server
    */
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
    /*
    Submitting form for network to server
    */
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
	/*
	Make characters for password <input> in/visible
	*/
    function showPass(checkboxEl,passwordEl){
        document.getElementById(passwordEl).type =(checkboxEl.checked)?"text":"password";
    }
    /*
    Depending on the state of 'button'
    either unhide its form and hide 'otherbutton'
    or hide its form and show 'otherbutton'
    */
    function toggle(button,otherbutton,wrapper){
        document.getElementById(wrapper).hidden =! document.getElementById(wrapper).hidden;
        //button.textContent = (button.textContent=="+")?"-":"+";
        if(button.textContent=="+"){
            button.textContent = "-";
            document.getElementById(otherbutton).hidden = true;
        }else{
            button.textContent ="+";
            document.getElementById(otherbutton).hidden = false;
        }
    }
    /*
    Send request to the server in order to change the state of the button
    */
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
    /*
    Send async json request using desired method
    */
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

