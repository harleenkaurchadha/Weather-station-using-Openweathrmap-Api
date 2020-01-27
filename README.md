# Weather-station-using-Openweathrmap-Api
OBJECTIVE
To create a weather station in which various weather parameters like (conditions such as haze,rain etc, min-max temperature, humidity and pressure) for a particular city and country to which it belongs to are being displayed on 16*2 lcd.
DESCRIPTION
• We have used nodemcu board(ESP8266-12) to communicate with Openweathermap api in order to create a weather station displaying various weather parameters.
• We are making api call with city id(particular id for every city) and capturing the server response(JSON format) to the request made.
• parsing of json array is done to capture the required data and being displayed on 16*2 lcd.


