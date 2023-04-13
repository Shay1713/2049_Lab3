void displayTime(long unsigned int inTime){ // needs to be always running in main while(1) loop
    // pass in global timer variable for parameter

    month = inTime/(157680000);
    monthMod = month % 12;
    day = inTime/(5184000);
    dayMod = day % 30;
    hour = inTime/(216000);
    hourMod = hour % 24;
    minute = inTime/(6000);
    minuteMod = minute % 60;
    second  = inTime/100;
    secondMod = second % 60;
    count = secondMod % 11;

    usnprintf(strDATE, 9, "%s %s", monthCurrent[monthMod], dayCurrent[dayMod]);
    usnprintf(strTIME, 9, "%02d:%02d:%02d", hourMod, minuteMod, secondMod);


    if ((count>=0) && (count <=2)){
        Graphics_drawStringCentered(&g_sContext, strDATE, 9, 48, 15, OPAQUE_TEXT);
        Graphics_flushBuffer(&g_sContext);
    }
    if ((count>=3) && (count <=5)) {
        Graphics_drawStringCentered(&g_sContext, strTIME, 9, 48, 15, OPAQUE_TEXT);
        Graphics_flushBuffer(&g_sContext);
    }

    printf("month %d, day %d, hour %d, minute %d, second %d, count %d\n", monthMod, dayMod, hourMod, minuteMod, secondMod, count);

}

void displayTemp(float inAvgTempC, float inAvgTempF, long unsigned int inTime){
    
    // actual averages done in another function, hence why they are parameter values
    // get average for tempC
    tempCTotal = inAvgTempC + tempCTotal; // make sure tempC and tempF Totals set to 0 globally initally
    tempFTotal = inAvgTempF + tempFTotal;
    // get average for tempF
    tempCAverage = tempCTotal / arrayIndex; // used as tempC parameter for displayOnLCD
    tempFAverage = tempFTotal / arrayIndex; // used as tempF parameter for displayOnLCD
    arrayIndex++; // have to divide by one more each time
    
    usnprintf(strTEMPC, 9, "%0.1f C\n", tempCAverage);
    usnprintf(strTEMPF, 9, "%0.1f F\n", tempFAverage);
    
    // for display purposes
    second  = inTime/100;
    secondMod = second % 60;
    count = secondMod % 11;
    
    if ((count>=6) && (count <=8)){
        Graphics_drawStringCentered(&g_sContext, strTEMPC, 9, 48, 15, OPAQUE_TEXT);
        Graphics_flushBuffer(&g_sContext);
    }
    if ((count>=9) && (count <=11)) {
        Graphics_drawStringCentered(&g_sContext, strTEMPF, 9, 48, 15, OPAQUE_TEXT);
        Graphics_flushBuffer(&g_sContext);
    }

}
