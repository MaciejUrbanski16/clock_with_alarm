"# clock_with_alarm" 


Specification
  - system measures the time and shows the current time on the display (both date and time)
  - displayed time is in the hh:mm:ss format and date in the dd.mm.yy format
  - the system has the ability to set the alarm at the time selected by the user
  - the clock should update time and date every second and display the current values
  - system has LCD display with 84x48 pixel resolution and 4 buttons
  - supply voltage = 3.3V
  - the system has 4 operating modes:
  
      – current time mode: updates date and time every second and displays the refreshed values;
pressing the button enabling alarm in this mode enables alarm. When the current time will
be equal to the alarm time previously set in the alarm setting mode buzzer will be triggered.
Alarm consists of 4 quick buzzer beeps with intervals of 200ms, then period of silence
lasting 500ms and again sequence starts with 4 buzzer beeps. Alarm sounds until the user
turns off buzzer with the button he used to activate it

    – manual set time mode: using the 3 buttons sequentially assigned to the hours, minutes and
seconds the user can manually set the current time. By pressing the appropriate button the
corresponding value is increased by 1: pressing the left button will increase the number of
hours by 1, the middle one will increase the number of minutes by 1 and the right one - the
number of seconds by 1

    – set alarm mode: similar as in the manual time setting mode, the user using 3 buttons
assigned to the respective components of the alarm time value has the ability to set the alarm
activation time. Pressing the appropriate button in this mode modifies the respective alarm
time component

    – manual set date mode: with 3 buttons assigned to the number of days, months and years
respectively the user can manually set the date. By pressing the appropriate button the
corresponding value increases by 1: pressing the left button increases the number of days by
1, pressing the middle one - the number of months by 1, the right one - the number of years
by 1

  - how buttons work:
      – 3 buttons assigned to the number of hours/days, minutes/months and seconds/years
respectively depending on the mode (explained above)
      – switching between modes is done by using the configuration button. The default work mode
is current time mode. When the user holds down the configuration button for 400ms while
in that mode it will switch to the set alarm mode. Holding the configuration button for
400ms while being in the set alarm mode switches it to the manual set time mode.
Holding the configuration button for 400ms in manual set time mode switches device to
the manual set date mode. Holding the configuration button in the manual set date mode
for 400ms transitions device back to the current time mode
      – quick press of a configuration button in any of the modes causes immediate return to the the
current time mode
      – to silence activated alarm user has to press any of the three buttons used to set hours,
minutes, seconds
  – 2 minutes after the activation of the alarm automatic silencing takes place if there is no user
reaction 
