void displayTime(long unsigned int inTime){ // needs to be always running in main while(1) loop
     // pass in global timer variable for parameter

     long unsigned int mSecondsInMonths = 2628000 * 100;
     long unsigned int mSecondsInDays = 86400 * 100;
     long unsigned int mSecondsInHours = 3600 * 100;
     long unsigned int mSecondsInMinutes = 60 * 100;
     long unsigned int mSecondsInSeconds = 100;

     long unsigned int month = (inTime/(mSecondsInMonths)) % 12;
     long unsigned int day = (inTime/(mSecondsInDays)) % 30;
     long unsigned int hour = (inTime/(mSecondsInHours)) % 24;
     long unsigned int minute = (inTime/(mSecondsInMinutes)) % 60;
     long unsigned int second  = inTime/mSecondsInSeconds;

     char months[] = {"JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"};
     char days[] = {"01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22","23", "24", "25", "26", "27", "28", "29", "30", "31"};


     char date[] = months[month] + " " + days[day];
     char time[] = hour + ":" + minute + ":" + second;


     Graphics_drawStringCentered(&g_sContext, date, 9, 48, 15, OPAQUE_TEXT);
     Graphics_drawStringCentered(&g_sContext, time, 9, 70, 15, OPAQUE_TEXT);
     Graphics_flushBuffer(&g_sContext);

 }
 // You need to take in the time instead of using a global variable because if we call the global variable then some time may have passed so we need to pass in an instance of the time

 void displayTemp(float inAvgTempC) {

     float inAvgTempF = (inAvgTempC * (9/5)) + 32;

     Graphics_drawStringCentered(&g_sContext, inAvgTempC, 9, 48, 15, OPAQUE_TEXT);
     Graphics_drawStringCentered(&g_sContext, inAvgTempF, 9, 70, 15, OPAQUE_TEXT);
     Graphics_flushBuffer(&g_sContext);

 }
