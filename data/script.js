// Get current sensor readings when the page loads
window.addEventListener('load', getReadings);

// Create Temperature Chart
var chartT = new Highcharts.Chart({
  chart: {
    renderTo: 'chart-temperature'
  },
  series: [
    {
      name: 'Temperature Chamber',
      type: 'line',
      color: '#00A6A6',
      marker: {
        symbol: 'circle',
        radius: 3,
        fillColor: '#00A6A6',
      },
      yAxis: 0 // Use the first y-axis
    },
    {
      name: 'RTD Value',
      type: 'line',
      color: '#FF5733',
      marker: {
        symbol: 'triangle',
        radius: 3,
        fillColor: '#FF5733',
      },
      yAxis: 1 // Use the second y-axis
    }
  ],
  title: {
    text: undefined
  },
  xAxis: {
    type: 'datetime',
    dateTimeLabelFormats: { second: '%H:%M:%S' }
  },
  yAxis: [
    {
      title: {
        text: 'Temperature [°C]',
        color: '#00A6A6'
      }
    },
    {
      title: {
        text: 'RTD Value [Ohm]',
        color:'#FF5733'
      },
      opposite: true // Position the second y-axis on the right
    }
  ],
  credits: {
    enabled: false
  }
});



//Plot temperature in the temperature chart
function plotTemperature(jsonValue) {

  var keys = Object.keys(jsonValue);
  console.log(keys);
  console.log(keys.length);

  for (var i = 0; i < keys.length; i++){
    var x = (new Date()).getTime();
    console.log(x);
    const key = keys[i];
    var y = Number(jsonValue[key]);
    console.log(y);

    if(chartT.series[i].data.length > 40) {
      chartT.series[i].addPoint([x, y], true, true, true);
    } else {
      chartT.series[i].addPoint([x, y], true, false, true);
    }

  }
}

// Function to get current readings on the webpage when it loads for the first time
function getReadings(){
  var xhr = new XMLHttpRequest();
  xhr.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      console.log(this.responseText);
    }
  };
  xhr.open("GET", "/readings", true);
  xhr.send();
}

if (!!window.EventSource) {
  var source = new EventSource('/events');

  source.addEventListener('open', function(e) {
    console.log("Events Connected");
  }, false);

  source.addEventListener('error', function(e) {
    if (e.target.readyState != EventSource.OPEN) {
      console.log("Events Disconnected");
    }
  }, false);

  source.addEventListener('message', function(e) {
    console.log("message", e.data);
  }, false);

  source.addEventListener('new_readings', function(e) {
    console.log("new_readings", e.data);
    var myObj = JSON.parse(e.data);
    console.log(myObj);
    plotTemperature(myObj);
  }, false);
}
