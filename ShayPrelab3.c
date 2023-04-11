void displayTime(long unsigned int inTime){ // needs to be always running in main while(1) loop
    // pass in global timer variable for parameter
    // each count on the timer is currently set to a 0.1 second interval rate
    month = inTime/(2629800);
    day = inTime/(86400);
    hour = inTime/(3600);
    minute = inTime/(60);
    second  = (inTime);

    //usnprintf(strDATE, 9, "%03s:%02s\n", monthCurrent[timeMonth - 1], dayCurrent[timeDay - 1]);
    //Graphics_drawStringCentered(&g_sContext, strTIME, 9, 48, 15, OPAQUE_TEXT);
    //Graphics_flushBuffer(&g_sContext);

    //usnprintf(strDATE, 9, "%02d:%02d:%02d\n", timeHour, timeMinute, timeSecond);
    //Graphics_drawStringCentered(&g_sContext, strDATE, 9, 48, 15, OPAQUE_TEXT);
    //Graphics_flushBuffer(&g_sContext);

}

void displayTemp(float inAvgTempC, float inAvgTempF){
    // get average for tempC
    tempCTotal = inAvgTempC + tempCTotal;
    tempFTotal = inAvgTempF + tempFTotal;
    // get average for tempF
    tempCAverage = tempCTotal / arrayIndex; // used as tempC parameter for displayOnLCD
    tempFAverage = tempFTotal / arrayIndex; // used as tempF parameter for displayOnLCD
    arrayIndex++;

    //usnprintf(strTEMPC, 9, "%03d.%0.1d C\n", tempC, tempCfrac); // working on
    //Graphics_drawStringCentered(&g_sContext, strTEMPC, 9, 48, 15, OPAQUE_TEXT);
    //Graphics_flushBuffer(&g_sContext);

    //usnprintf(strTEMPF, 9, "%03d.%0.1d F\n", tempF, tempFfrac);
    //Graphics_drawStringCentered(&g_sContext, strTEMPF, 9, 48, 15, OPAQUE_TEXT);
    //Graphics_flushBuffer(&g_sContext);
}
