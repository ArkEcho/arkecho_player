var lh = "localhost";
var lhIp = "127.0.0.1";
var net = "192.168.0.59"; // Adresse im Netzwerk
var net2 = "192.168.178.20"; // Adresse LGH
var webSocket = new WebSocket("ws://" + lh + ":1000");

webSocket.onopen = function (evt) {
    alert("Connected..");
};

webSocket.onclose = function (evt) {
    alert("Disconnected...");
};

webSocket.onmessage = function (evt) {
    var json = JSON.parse(evt.data);
    var out = "Type: " + json.Typ + " Nachricht: " + json.Message;
    alert(out);
};

webSocket.onerror = function (evt) {
    alert("Error...");
};

function send() {
    var typ = document.getElementById("type");
    var valTyp = typ.value;

    var msg = document.getElementById("msg");
    var valMsg = msg.value;

    var json = '{ "Type": '+valTyp+', "Message": "'+valMsg+'" }';
    webSocket.send(json);
}
