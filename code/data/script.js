let temp = 0.0;
let humi = 0.0;
let mois = 0;
let rssi = 0;

let HttpClient = function() {
    this.get = function(aUrl, aCallback) {
        let httpReq = new XMLHttpRequest();
        httpReq.onreadystatechange = () => { 
            if (httpReq.readyState == 4 && httpReq.status == 200)
                aCallback(httpReq.responseText);
        }
        httpReq.open( "GET", aUrl, true );            
        httpReq.send( null );
    };
}
let client = new HttpClient();
let ForceUpdate = () => {
    console.log("> Forcing update...")
    client.get('/update', () => {
        RefreshData();
    });
}
let RefreshData = () => {
    console.log("> Getting fresh data...")
    client.get('/temp', (response) => {
        temp = Math.round(response)
        console.log("> Temperature:" + response);
        UpdateDOM();
    });
    client.get('/humi', (response) => {
        humi = Math.round(response)
        console.log("> Humidity:" + response);
        UpdateDOM();
    });
    client.get('/mois', (response) => {
        mois = Math.round(response)
        console.log("> Moisure:" + response);
        UpdateDOM();
    });
    client.get('/rssi', (response) => {
        rssi = Math.round(response)
        console.log("> RSSI:" + response);
        UpdateDOM();
    });
}
let UpdateDOM = () => {
    document.getElementById("temp").innerHTML = `${temp}°C`;
    document.getElementById("humi").innerHTML =`${humi}%`;
    document.getElementById("mois").innerHTML = `${mois}`;
    document.getElementById("rssi").innerHTML = `${rssi}`;
    document.title = `🪴 ${temp}°C, ${humi}%, ${mois} // Plant Best Friend / ESP8266 IoT`; 
}
ForceUpdate();
setInterval(RefreshData, 10000); // 10s
setInterval(ForceUpdate, 55000); // 55s