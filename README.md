# Volleyball-scoreboard
Simple hardware to keep the volleyball score via a small remote operated by one of the players

A Youtube video about this project here https://youtu.be/qLoRw70-ni4

#Materials

Panel: https://www.buyapi.ca/product/medium-16x32-rgb-led-matrix-panel/ >> Wiring https://learn.adafruit.com/32x16-32x32-rgb-led-matrix/new-wiring

Battery: https://www.youtube.com/watch?v=bOJVbtUL_hg 

Remote control: https://www.aliexpress.com/item/32962560726.html?spm=a2g0s.9042311.0.0.5f4e4c4dmDa23P

Arduino UNO R3 https://www.aliexpress.com/item/32848546164.html?spm=2114.search0204.3.1.115f6374utWwzm&s=p&ws_ab_test=searchweb0_0%2Csearchweb201602_1_10065_10068_10130_10547_319_10059_10884_317_10548_10887_10696_321_322_10084_453_10083_454_10103_10618_10307_537_536%2Csearchweb201603_60%2CppcSwitch_0&algo_expid=88e54d9c-e84a-4e37-89d0-ad275945d81a-0&algo_pvid=88e54d9c-e84a-4e37-89d0-ad275945d81a&transAbTest=ae803_3


3D printed case : https://www.thingiverse.com/thing:1112772/

rubber feet/wires you can source them yourself


1. Wire the screen as shown in the wiring link from adafruit

2. After completing step 1 you will notice that there are only few pins available therefore we need to use two digital and two analogous pins for the remote wiring; please wire them as follow
  
  int digitalPinA = 13; (Button A)  
  
  int digitalPinB = 12; (Button B)
  
  int signalPin0 = A4; (Button C)
  
  int signalPin1 = A5; (Button D)
  
3. I opened the battery from Dollarama store remove the USB host and wire the 5V output into both my Arduino Uno power socket (that I also removed) and into the positive input for the LED panel. 
   I cut the positive lead of the power and attached a toggle switch to power on / off the board

5. upload the code

6. mount everything inside as the displayed picture. The case is for a Raspberry pi so I had to adapt. 
    - I used the power socket to install the on off toggle switch.
	- I used the USB opening to expose the Arduino USB socket (for future firmware updates)
	- I used the LAN opening to vertically mount the mini USB in order to charge the battery. My case was printed from blue PLA and it is pretty transparent allowing the user to see the charge LED blinking.

7. Test it. In my tests a full battery was good for 3 hours. Dollarama battery is only a 1800Mah so using a 2600Mah should provide at least an extra hour.	

Possible future improvements
 - a new case for this very purpose 
 - multi sport (e.g. basketball, tennis)
 - a better battery
 - use the Arduino USB socket for charging rather than the mini USB.
 - and last but not least display at some point the batter level since it dies suddenly when a threshold in the battery voltage is being reached.
