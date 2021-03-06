/**
vim: ts=4
vim: shiftwidth=4
*/

//***********************************************************************/
//*	Data for Selectbox Controls							*/
//***********************************************************************/
#if (0)
	var monthList = new Array();	//	list of months and days for non-leap year
	var i = 0;
	monthList[i++] = new month("January", 31, "Jan");
	monthList[i++] = new month("February", 28, "Feb");
	monthList[i++] = new month("March", 31, "Mar");
	monthList[i++] = new month("April", 30, "Apr");
	monthList[i++] = new month("May", 31, "May");
	monthList[i++] = new month("June", 30, "Jun");
	monthList[i++] = new month("July", 31, "Jul");
	monthList[i++] = new month("August", 31, "Aug");
	monthList[i++] = new month("September", 30, "Sep");
	monthList[i++] = new month("October", 31, "Oct");
	monthList[i++] = new month("November", 30, "Nov");
	monthList[i++] = new month("December", 31, "Dec");
#endif

//*********************************************************************/

#if (0)
	var YesNo = new Array();	//Daylight Saving array	
	i=0;
	YesNo[i++] = new ans("No",0);
	YesNo[i++] = new ans("Yes",60);
#endif

//*********************************************************************/

#if (0)
	var City = new Array();

	j = 0;
	City[j++] = new city("Enter Lat/Long -->",0,0,0);
	City[j++] = new city("",0,0,0);
	City[j++] = new city("US CITIES",0,0,0);
	City[j++] = new city("Albuquerque, NM", 35.0833,106.65,7);
	City[j++] = new city("Anchorage, AK", 61.217, 149.90,9);
	City[j++] = new city("Atlanta, GA", 33.733, 84.383, 5);
#endif

/**********************************************************************/
/** Is the given 4-digit year a leap year? */
static bool IsLeapYear(	const int	Year)
{
	return (Year % 4 == 0 && Year % 100 != 0) || Year % 400 == 0;
}

/***********************************************************************/
/***********************************************************************/
static int ifloor(const double v)
{
	return floor(v);
}

/***********************************************************************/
/**
Finds numerical day-of-year from given month, day of month and leap year.
\param[in] Month		Month, January = 1
\param[in] DayOfMonth	1 - 31
\param[in] IsLeapYear	Is current year leap year?
\return  The numerical day of year
*/
static int DayOfYear(int Month, int DayOfMonth, bool IsLeapYear)
{
	const int k = IsLeapYear ? 1 : 2;
	const int r = ifloor((275.0 * Month)/9.0) - k * ifloor((Month + 9.0)/12.0) + DayOfMonth - 30;
	return r;
}

//***********************************************************************/
//* Name:    calcJD									*/
//* Type:    Function									*/
//* Purpose: Julian day from calendar day						*/
//* Arguments:										*/
//*   year : 4 digit year								*/
//*   month: January = 1								*/
//*   day  : 1 - 31									*/
//* Return value:										*/
//*   The Julian day corresponding to the date					*/
//* Note:											*/
//*   Number is returned for start of day.  Fractional days should be	*/
//*   added later.									*/
//***********************************************************************/

	function calcJD(year, month, day)
	{
		if (month <= 2) {
			year -= 1;
			month += 12;
		}
		var A = Math.floor(year/100);
		var B = 2 - A + Math.floor(A/4);

		var JD = Math.floor(365.25*(year + 4716)) + Math.floor(30.6001*(month+1)) + day + B - 1524.5;
		return JD;
	}



//***********************************************************************/
//* Name:    calcDateFromJD								*/
//* Type:    Function									*/
//* Purpose: Calendar date from Julian Day					*/
//* Arguments:										*/
//*   jd   : Julian Day									*/
//* Return value:										*/
//*   String date in the form DD-MONTHNAME-YYYY					*/
//* Note:											*/
//***********************************************************************/

	function calcDateFromJD(jd)
	{
		var z = Math.floor(jd + 0.5);
		var f = (jd + 0.5) - z;

		if (z < 2299161) {
			var A = z;
		} else {
			alpha = Math.floor((z - 1867216.25)/36524.25);
			var A = z + 1 + alpha - Math.floor(alpha/4);
		}

		var B = A + 1524;
		var C = Math.floor((B - 122.1)/365.25);
		var D = Math.floor(365.25 * C);
		var E = Math.floor((B - D)/30.6001);

		var day = B - D - Math.floor(30.6001 * E) + f;
		var month = (E < 14) ? E - 1 : E - 13;
		var year = (month > 2) ? C - 4716 : C - 4715;

		// alert ("date: " + day + "-" + monthList[month-1].name + "-" + year);
		return (day + "-" + monthList[month-1].name + "-" + year);
	}


//***********************************************************************/
//* Name:    calcDayFromJD								*/
//* Type:    Function									*/
//* Purpose: Calendar day (minus year) from Julian Day			*/
//* Arguments:										*/
//*   jd   : Julian Day									*/
//* Return value:										*/
//*   String date in the form DD-MONTH						*/
//***********************************************************************/

	function calcDayFromJD(jd)
	{
		var z = Math.floor(jd + 0.5);
		var f = (jd + 0.5) - z;

		if (z < 2299161) {
			var A = z;
		} else {
			alpha = Math.floor((z - 1867216.25)/36524.25);
			var A = z + 1 + alpha - Math.floor(alpha/4);
		}

		var B = A + 1524;
		var C = Math.floor((B - 122.1)/365.25);
		var D = Math.floor(365.25 * C);
		var E = Math.floor((B - D)/30.6001);

		var day = B - D - Math.floor(30.6001 * E) + f;
		var month = (E < 14) ? E - 1 : E - 13;
		var year = (month > 2) ? C - 4716 : C - 4715;

		return ((day<10 ? "0" : "") + day + monthList[month-1].abbr);
	}


//***********************************************************************/
//* Name:    calcTimeJulianCent							*/
//* Type:    Function									*/
//* Purpose: convert Julian Day to centuries since J2000.0.			*/
//* Arguments:										*/
//*   jd : the Julian Day to convert						*/
//* Return value:										*/
//*   the T value corresponding to the Julian Day				*/
//***********************************************************************/

	function calcTimeJulianCent(jd)
	{
		var T = (jd - 2451545.0)/36525.0;
		return T;
	}


//***********************************************************************/
//* Name:    calcJDFromJulianCent							*/
//* Type:    Function									*/
//* Purpose: convert centuries since J2000.0 to Julian Day.			*/
//* Arguments:										*/
//*   t : number of Julian centuries since J2000.0				*/
//* Return value:										*/
//*   the Julian Day corresponding to the t value				*/
//***********************************************************************/

	function calcJDFromJulianCent(t)
	{
		var JD = t * 36525.0 + 2451545.0;
		return JD;
	}


//***********************************************************************/
//* Name:    calGeomMeanLongSun							*/
//* Type:    Function									*/
//* Purpose: calculate the Geometric Mean Longitude of the Sun		*/
//* Arguments:										*/
//*   t : number of Julian centuries since J2000.0				*/
//* Return value:										*/
//*   the Geometric Mean Longitude of the Sun in degrees			*/
//***********************************************************************/

	function calcGeomMeanLongSun(t)
	{
		var L0 = 280.46646 + t * (36000.76983 + 0.0003032 * t);
		while(L0 > 360.0)
		{
			L0 -= 360.0;
		}
		while(L0 < 0.0)
		{
			L0 += 360.0;
		}
		return L0;		// in degrees
	}


//***********************************************************************/
//* Name:    calGeomAnomalySun							*/
//* Type:    Function									*/
//* Purpose: calculate the Geometric Mean Anomaly of the Sun		*/
//* Arguments:										*/
//*   t : number of Julian centuries since J2000.0				*/
//* Return value:										*/
//*   the Geometric Mean Anomaly of the Sun in degrees			*/
//***********************************************************************/

	function calcGeomMeanAnomalySun(t)
	{
		var M = 357.52911 + t * (35999.05029 - 0.0001537 * t);
		return M;		// in degrees
	}

//***********************************************************************/
//* Name:    calcEccentricityEarthOrbit						*/
//* Type:    Function									*/
//* Purpose: calculate the eccentricity of earth's orbit			*/
//* Arguments:										*/
//*   t : number of Julian centuries since J2000.0				*/
//* Return value:										*/
//*   the unitless eccentricity							*/
//***********************************************************************/


	function calcEccentricityEarthOrbit(t)
	{
		var e = 0.016708634 - t * (0.000042037 + 0.0000001267 * t);
		return e;		// unitless
	}

//***********************************************************************/
//* Name:    calcSunEqOfCenter							*/
//* Type:    Function									*/
//* Purpose: calculate the equation of center for the sun			*/
//* Arguments:										*/
//*   t : number of Julian centuries since J2000.0				*/
//* Return value:										*/
//*   in degrees										*/
//***********************************************************************/


	function calcSunEqOfCenter(t)
	{
		var m = calcGeomMeanAnomalySun(t);

		var mrad = degToRad(m);
		var sinm = Math.sin(mrad);
		var sin2m = Math.sin(mrad+mrad);
		var sin3m = Math.sin(mrad+mrad+mrad);

		var C = sinm * (1.914602 - t * (0.004817 + 0.000014 * t)) + sin2m * (0.019993 - 0.000101 * t) + sin3m * 0.000289;
		return C;		// in degrees
	}

//***********************************************************************/
//* Name:    calcSunTrueLong								*/
//* Type:    Function									*/
//* Purpose: calculate the true longitude of the sun				*/
//* Arguments:										*/
//*   t : number of Julian centuries since J2000.0				*/
//* Return value:										*/
//*   sun's true longitude in degrees						*/
//***********************************************************************/


	function calcSunTrueLong(t)
	{
		var l0 = calcGeomMeanLongSun(t);
		var c = calcSunEqOfCenter(t);

		var O = l0 + c;
		return O;		// in degrees
	}

//***********************************************************************/
//* Name:    calcSunTrueAnomaly							*/
//* Type:    Function									*/
//* Purpose: calculate the true anamoly of the sun				*/
//* Arguments:										*/
//*   t : number of Julian centuries since J2000.0				*/
//* Return value:										*/
//*   sun's true anamoly in degrees							*/
//***********************************************************************/

	function calcSunTrueAnomaly(t)
	{
		var m = calcGeomMeanAnomalySun(t);
		var c = calcSunEqOfCenter(t);

		var v = m + c;
		return v;		// in degrees
	}

//***********************************************************************/
//* Name:    calcSunRadVector								*/
//* Type:    Function									*/
//* Purpose: calculate the distance to the sun in AU				*/
//* Arguments:										*/
//*   t : number of Julian centuries since J2000.0				*/
//* Return value:										*/
//*   sun radius vector in AUs							*/
//***********************************************************************/

	function calcSunRadVector(t)
	{
		var v = calcSunTrueAnomaly(t);
		var e = calcEccentricityEarthOrbit(t);
 
		var R = (1.000001018 * (1 - e * e)) / (1 + e * Math.cos(degToRad(v)));
		return R;		// in AUs
	}

//***********************************************************************/
//* Name:    calcSunApparentLong							*/
//* Type:    Function									*/
//* Purpose: calculate the apparent longitude of the sun			*/
//* Arguments:										*/
//*   t : number of Julian centuries since J2000.0				*/
//* Return value:										*/
//*   sun's apparent longitude in degrees						*/
//***********************************************************************/

	function calcSunApparentLong(t)
	{
		var o = calcSunTrueLong(t);

		var omega = 125.04 - 1934.136 * t;
		var lambda = o - 0.00569 - 0.00478 * Math.sin(degToRad(omega));
		return lambda;		// in degrees
	}

//***********************************************************************/
//* Name:    calcMeanObliquityOfEcliptic						*/
//* Type:    Function									*/
//* Purpose: calculate the mean obliquity of the ecliptic			*/
//* Arguments:										*/
//*   t : number of Julian centuries since J2000.0				*/
//* Return value:										*/
//*   mean obliquity in degrees							*/
//***********************************************************************/

	function calcMeanObliquityOfEcliptic(t)
	{
		var seconds = 21.448 - t*(46.8150 + t*(0.00059 - t*(0.001813)));
		var e0 = 23.0 + (26.0 + (seconds/60.0))/60.0;
		return e0;		// in degrees
	}

//***********************************************************************/
//* Name:    calcObliquityCorrection						*/
//* Type:    Function									*/
//* Purpose: calculate the corrected obliquity of the ecliptic		*/
//* Arguments:										*/
//*   t : number of Julian centuries since J2000.0				*/
//* Return value:										*/
//*   corrected obliquity in degrees						*/
//***********************************************************************/

	function calcObliquityCorrection(t)
	{
		var e0 = calcMeanObliquityOfEcliptic(t);

		var omega = 125.04 - 1934.136 * t;
		var e = e0 + 0.00256 * Math.cos(degToRad(omega));
		return e;		// in degrees
	}

//***********************************************************************/
//* Name:    calcSunRtAscension							*/
//* Type:    Function									*/
//* Purpose: calculate the right ascension of the sun				*/
//* Arguments:										*/
//*   t : number of Julian centuries since J2000.0				*/
//* Return value:										*/
//*   sun's right ascension in degrees						*/
//***********************************************************************/

	function calcSunRtAscension(t)
	{
		var e = calcObliquityCorrection(t);
		var lambda = calcSunApparentLong(t);
 
		var tananum = (Math.cos(degToRad(e)) * Math.sin(degToRad(lambda)));
		var tanadenom = (Math.cos(degToRad(lambda)));
		var alpha = radToDeg(Math.atan2(tananum, tanadenom));
		return alpha;		// in degrees
	}

//***********************************************************************/
//* Name:    calcSunDeclination							*/
//* Type:    Function									*/
//* Purpose: calculate the declination of the sun				*/
//* Arguments:										*/
//*   t : number of Julian centuries since J2000.0				*/
//* Return value:										*/
//*   sun's declination in degrees							*/
//***********************************************************************/

	function calcSunDeclination(t)
	{
		var e = calcObliquityCorrection(t);
		var lambda = calcSunApparentLong(t);

		var sint = Math.sin(degToRad(e)) * Math.sin(degToRad(lambda));
		var theta = radToDeg(Math.asin(sint));
		return theta;		// in degrees
	}

//***********************************************************************/
//* Name:    calcEquationOfTime							*/
//* Type:    Function									*/
//* Purpose: calculate the difference between true solar time and mean	*/
//*		solar time									*/
//* Arguments:										*/
//*   t : number of Julian centuries since J2000.0				*/
//* Return value:										*/
//*   equation of time in minutes of time						*/
//***********************************************************************/

	function calcEquationOfTime(t)
	{
		var epsilon = calcObliquityCorrection(t);
		var l0 = calcGeomMeanLongSun(t);
		var e = calcEccentricityEarthOrbit(t);
		var m = calcGeomMeanAnomalySun(t);

		var y = Math.tan(degToRad(epsilon)/2.0);
		y *= y;

		var sin2l0 = Math.sin(2.0 * degToRad(l0));
		var sinm   = Math.sin(degToRad(m));
		var cos2l0 = Math.cos(2.0 * degToRad(l0));
		var sin4l0 = Math.sin(4.0 * degToRad(l0));
		var sin2m  = Math.sin(2.0 * degToRad(m));

		var Etime = y * sin2l0 - 2.0 * e * sinm + 4.0 * e * y * sinm * cos2l0
				- 0.5 * y * y * sin4l0 - 1.25 * e * e * sin2m;

		return radToDeg(Etime)*4.0;	// in minutes of time
	}

//***********************************************************************/
//* Name:    calcHourAngleSunrise							*/
//* Type:    Function									*/
//* Purpose: calculate the hour angle of the sun at sunrise for the	*/
//*			latitude								*/
//* Arguments:										*/
//*   lat : latitude of observer in degrees					*/
//*	solarDec : declination angle of sun in degrees				*/
//* Return value:										*/
//*   hour angle of sunrise in radians						*/
//***********************************************************************/

	function calcHourAngleSunrise(lat, solarDec)
	{
		var latRad = degToRad(lat);
		var sdRad  = degToRad(solarDec)

		var HAarg = (Math.cos(degToRad(90.833))/(Math.cos(latRad)*Math.cos(sdRad))-Math.tan(latRad) * Math.tan(sdRad));

		var HA = (Math.acos(Math.cos(degToRad(90.833))/(Math.cos(latRad)*Math.cos(sdRad))-Math.tan(latRad) * Math.tan(sdRad)));

		return HA;		// in radians
	}

//***********************************************************************/
//* Name:    calcHourAngleSunset							*/
//* Type:    Function									*/
//* Purpose: calculate the hour angle of the sun at sunset for the	*/
//*			latitude								*/
//* Arguments:										*/
//*   lat : latitude of observer in degrees					*/
//*	solarDec : declination angle of sun in degrees				*/
//* Return value:										*/
//*   hour angle of sunset in radians						*/
//***********************************************************************/

	function calcHourAngleSunset(lat, solarDec)
	{
		var latRad = degToRad(lat);
		var sdRad  = degToRad(solarDec)

		var HAarg = (Math.cos(degToRad(90.833))/(Math.cos(latRad)*Math.cos(sdRad))-Math.tan(latRad) * Math.tan(sdRad));

		var HA = (Math.acos(Math.cos(degToRad(90.833))/(Math.cos(latRad)*Math.cos(sdRad))-Math.tan(latRad) * Math.tan(sdRad)));

		return -HA;		// in radians
	}


//***********************************************************************/
//* Name:    calcSunriseUTC								*/
//* Type:    Function									*/
//* Purpose: calculate the Universal Coordinated Time (UTC) of sunrise	*/
//*			for the given day at the given location on earth	*/
//* Arguments:										*/
//*   JD  : julian day									*/
//*   latitude : latitude of observer in degrees				*/
//*   longitude : longitude of observer in degrees				*/
//* Return value:										*/
//*   time in minutes from zero Z							*/
//***********************************************************************/

	function calcSunriseUTC(JD, latitude, longitude)
	{
		var t = calcTimeJulianCent(JD);

		// *** Find the time of solar noon at the location, and use
        //     that declination. This is better than start of the 
        //     Julian day

		var noonmin = calcSolNoonUTC(t, longitude);
		var tnoon = calcTimeJulianCent (JD+noonmin/1440.0);

		// *** First pass to approximate sunrise (using solar noon)

		var eqTime = calcEquationOfTime(tnoon);
		var solarDec = calcSunDeclination(tnoon);
		var hourAngle = calcHourAngleSunrise(latitude, solarDec);

		var delta = longitude - radToDeg(hourAngle);
		var timeDiff = 4 * delta;	// in minutes of time
		var timeUTC = 720 + timeDiff - eqTime;	// in minutes

		// alert("eqTime = " + eqTime + "\nsolarDec = " + solarDec + "\ntimeUTC = " + timeUTC);

		// *** Second pass includes fractional jday in gamma calc

		var newt = calcTimeJulianCent(calcJDFromJulianCent(t) + timeUTC/1440.0); 
		eqTime = calcEquationOfTime(newt);
		solarDec = calcSunDeclination(newt);
		hourAngle = calcHourAngleSunrise(latitude, solarDec);
		delta = longitude - radToDeg(hourAngle);
		timeDiff = 4 * delta;
		timeUTC = 720 + timeDiff - eqTime; // in minutes

		// alert("eqTime = " + eqTime + "\nsolarDec = " + solarDec + "\ntimeUTC = " + timeUTC);

		return timeUTC;
	}

//***********************************************************************/
//* Name:    calcSolNoonUTC								*/
//* Type:    Function									*/
//* Purpose: calculate the Universal Coordinated Time (UTC) of solar	*/
//*		noon for the given day at the given location on earth		*/
//* Arguments:										*/
//*   t : number of Julian centuries since J2000.0				*/
//*   longitude : longitude of observer in degrees				*/
//* Return value:										*/
//*   time in minutes from zero Z							*/
//***********************************************************************/

	function calcSolNoonUTC(t, longitude)
	{
		// First pass uses approximate solar noon to calculate eqtime
		var tnoon = calcTimeJulianCent(calcJDFromJulianCent(t) + longitude/360.0);
		var eqTime = calcEquationOfTime(tnoon);
		var solNoonUTC = 720 + (longitude * 4) - eqTime; // min

		var newt = calcTimeJulianCent(calcJDFromJulianCent(t) -0.5 + solNoonUTC/1440.0); 

		eqTime = calcEquationOfTime(newt);
		// var solarNoonDec = calcSunDeclination(newt);
		solNoonUTC = 720 + (longitude * 4) - eqTime; // min
		
		return solNoonUTC;
	}

//***********************************************************************/
//* Name:    calcSunsetUTC								*/
//* Type:    Function									*/
//* Purpose: calculate the Universal Coordinated Time (UTC) of sunset	*/
//*			for the given day at the given location on earth	*/
//* Arguments:										*/
//*   JD  : julian day									*/
//*   latitude : latitude of observer in degrees				*/
//*   longitude : longitude of observer in degrees				*/
//* Return value:										*/
//*   time in minutes from zero Z							*/
//***********************************************************************/

	function calcSunsetUTC(JD, latitude, longitude)
	{
		var t = calcTimeJulianCent(JD);

		// *** Find the time of solar noon at the location, and use
        //     that declination. This is better than start of the 
        //     Julian day

		var noonmin = calcSolNoonUTC(t, longitude);
		var tnoon = calcTimeJulianCent (JD+noonmin/1440.0);

		// First calculates sunrise and approx length of day

		var eqTime = calcEquationOfTime(tnoon);
		var solarDec = calcSunDeclination(tnoon);
		var hourAngle = calcHourAngleSunset(latitude, solarDec);

		var delta = longitude - radToDeg(hourAngle);
		var timeDiff = 4 * delta;
		var timeUTC = 720 + timeDiff - eqTime;

		// first pass used to include fractional day in gamma calc

		var newt = calcTimeJulianCent(calcJDFromJulianCent(t) + timeUTC/1440.0); 
		eqTime = calcEquationOfTime(newt);
		solarDec = calcSunDeclination(newt);
		hourAngle = calcHourAngleSunset(latitude, solarDec);

		delta = longitude - radToDeg(hourAngle);
		timeDiff = 4 * delta;
		timeUTC = 720 + timeDiff - eqTime; // in minutes

		return timeUTC;
	}


//*********************************************************************/

// Returns the decimal latitude from the degrees, minutes and seconds entered 
// into the form	

	function getLatitude(latLongForm)
	{
		var neg = 0;
		var strLatDeg = latLongForm["latDeg"].value;
		var degs = parseFloat(latLongForm["latDeg"].value);
		if (latLongForm["latDeg"].value.charAt(0) == '-') 
		{
			neg = 1;
		}

		if (strLatDeg.indexOf(".") != -1) 
		{
			latLongForm["latMin"].value = 0;
			latLongForm["latSec"].value = 0;
		}

		if(latLongForm["latMin"].value == "")
		{
			latLongForm["latMin"].value = 0;
		}
		if(latLongForm["latSec"].value == "")
		{
			latLongForm["latSec"].value = 0;
		}

		var mins = parseFloat(latLongForm["latMin"].value);

		var secs = parseFloat(latLongForm["latSec"].value);

		if(neg != 1)
		{
			var decLat = degs + (mins / 60) + (secs / 3600);
		} else if(neg == 1)
		{
			var decLat = degs - (mins / 60) - (secs / 3600);
		} else 
		{
			return -9999;
		}
		return decLat;
	}	


//*********************************************************************/

// Returns the decimal longitude from the degrees, minutes and seconds entered 
// into the form	

	function getLongitude(latLongForm)
	{
		var neg = 0;
		var strLonDeg = latLongForm["lonDeg"].value;
		var degs = parseFloat(latLongForm["lonDeg"].value);
		if (latLongForm["lonDeg"].value.charAt(0) == '-') 
		{
			neg = 1;
		}

		if (strLonDeg.indexOf(".") != -1) 
		{
			latLongForm["lonMin"].value = 0;
			latLongForm["lonSec"].value = 0;
		}

		if(latLongForm["lonMin"].value == "")
		{
			latLongForm["lonMin"].value = 0;
		}
		if(latLongForm["lonSec"].value == "")
		{
			latLongForm["lonSec"].value = 0;
		}

		var mins = parseFloat(latLongForm["lonMin"].value);
		var secs = parseFloat(latLongForm["lonSec"].value);
		var decLon = degs + (mins / 60) + (secs / 3600);

		if(neg != 1)
		{
			var decLon = degs + (mins / 60) + (secs / 3600);
		} else if(neg == 1)
		{
			var decLon = degs - (mins / 60) - (secs / 3600);
		} else 
		{
			return -9999;
		}
		return decLon;
	}	


//***********************************************************************/
//* Name:    findRecentSunrise							*/
//* Type:    Function									*/
//* Purpose: calculate the julian day of the most recent sunrise		*/
//*		starting from the given day at the given location on earth	*/
//* Arguments:										*/
//*   JD  : julian day									*/
//*   latitude : latitude of observer in degrees				*/
//*   longitude : longitude of observer in degrees				*/
//* Return value:										*/
//*   julian day of the most recent sunrise					*/
//***********************************************************************/

	function findRecentSunrise(jd, latitude, longitude)
	{
		var julianday = jd;

		var time = calcSunriseUTC(julianday, latitude, longitude);
		while(!isNumber(time)){
			julianday -= 1.0;
			time = calcSunriseUTC(julianday, latitude, longitude);
		}

		return julianday;
	}


//***********************************************************************/
//* Name:    findRecentSunset								*/
//* Type:    Function									*/
//* Purpose: calculate the julian day of the most recent sunset		*/
//*		starting from the given day at the given location on earth	*/
//* Arguments:										*/
//*   JD  : julian day									*/
//*   latitude : latitude of observer in degrees				*/
//*   longitude : longitude of observer in degrees				*/
//* Return value:										*/
//*   julian day of the most recent sunset					*/
//***********************************************************************/

	function findRecentSunset(jd, latitude, longitude)
	{
		var julianday = jd;

		var time = calcSunsetUTC(julianday, latitude, longitude);
		while(!isNumber(time)){
			julianday -= 1.0;
			time = calcSunsetUTC(julianday, latitude, longitude);
		}

		return julianday;
	}


//***********************************************************************/
//* Name:    findNextSunrise								*/
//* Type:    Function									*/
//* Purpose: calculate the julian day of the next sunrise			*/
//*		starting from the given day at the given location on earth	*/
//* Arguments:										*/
//*   JD  : julian day									*/
//*   latitude : latitude of observer in degrees				*/
//*   longitude : longitude of observer in degrees				*/
//* Return value:										*/
//*   julian day of the next sunrise						*/
//***********************************************************************/

	function findNextSunrise(jd, latitude, longitude)
	{
		var julianday = jd;

		var time = calcSunriseUTC(julianday, latitude, longitude);
		while(!isNumber(time)){
			julianday += 1.0;
			time = calcSunriseUTC(julianday, latitude, longitude);
		}

		return julianday;
	}


//***********************************************************************/
//* Name:    findNextSunset								*/
//* Type:    Function									*/
//* Purpose: calculate the julian day of the next sunset			*/
//*		starting from the given day at the given location on earth	*/
//* Arguments:										*/
//*   JD  : julian day									*/
//*   latitude : latitude of observer in degrees				*/
//*   longitude : longitude of observer in degrees				*/
//* Return value:										*/
//*   julian day of the next sunset							*/
//***********************************************************************/

	function findNextSunset(jd, latitude, longitude)
	{
		var julianday = jd;

		var time = calcSunsetUTC(julianday, latitude, longitude);
		while(!isNumber(time)){
			julianday += 1.0;
			time = calcSunsetUTC(julianday, latitude, longitude);
		}

		return julianday;
	}

//***********************************************************************/
//* Name:    timeString									*/
//* Type:    Function									*/
//* Purpose: convert time of day in minutes to a zero-padded string	*/
//*		suitable for printing to the form text fields			*/
//* Arguments:										*/
//*   minutes : time of day in minutes						*/
//* Return value:										*/
//*   string of the format HH:MM:SS, minutes and seconds are zero padded*/
//***********************************************************************/

	function timeString(minutes)
	// timeString returns a zero-padded string (HH:MM:SS) given time in minutes
	{
		var floatHour = minutes / 60.0;
		var hour = Math.floor(floatHour);
		var floatMinute = 60.0 * (floatHour - Math.floor(floatHour));
		var minute = Math.floor(floatMinute);
		var floatSec = 60.0 * (floatMinute - Math.floor(floatMinute));
		var second = Math.floor(floatSec + 0.5);

		var timeStr = hour + ":";
		if (minute < 10)	//	i.e. only one digit
			timeStr += "0" + minute + ":";
		else
			timeStr += minute + ":";
		if (second < 10)	//	i.e. only one digit
			timeStr += "0" + second;
		else
			timeStr += second;

		return timeStr;
	}


//***********************************************************************/
//* Name:    timeStringShortAMPM							*/
//* Type:    Function									*/
//* Purpose: convert time of day in minutes to a zero-padded string	*/
//*		suitable for printing to the form text fields.  If time	*/
//*		crosses a day boundary, date is appended.				*/
//* Arguments:										*/
//*   minutes : time of day in minutes						*/
//*   JD  : julian day									*/
//* Return value:										*/
//*   string of the format HH:MM[AM/PM] (DDMon)					*/
//***********************************************************************/

// timeStringShortAMPM returns a zero-padded string (HH:MM *M) given time in 
// minutes and appends short date if time is > 24 or < 0, resp.

	function timeStringShortAMPM(minutes, JD)
	{
		var julianday = JD;
		var floatHour = minutes / 60.0;
		var hour = Math.floor(floatHour);
		var floatMinute = 60.0 * (floatHour - Math.floor(floatHour));
		var minute = Math.floor(floatMinute);
		var floatSec = 60.0 * (floatMinute - Math.floor(floatMinute));
		var second = Math.floor(floatSec + 0.5);
		var PM = false;

		minute += (second >= 30)? 1 : 0;

		if (minute >= 60) 
		{
			minute -= 60;
			hour ++;
		}

		var daychange = false;
		if (hour > 23) 
		{
			hour -= 24;
			daychange = true;
			julianday += 1.0;
		}

		if (hour < 0)
		{
			hour += 24;
			daychange = true;
			julianday -= 1.0;
		}

		if (hour > 12)
		{
			hour -= 12;
			PM = true;
		}

            if (hour == 12)
		{
              PM = true;
            }

		if (hour == 0)
		{
			PM = false;
			hour = 12;
		}

		var timeStr = hour + ":";
		if (minute < 10)	//	i.e. only one digit
			timeStr += "0" + minute + ((PM)?"PM":"AM");
		else
			timeStr += "" + minute + ((PM)?"PM":"AM");

		if (daychange) return timeStr + " " + calcDayFromJD(julianday);
		return timeStr;
	}


//***********************************************************************/
//* Name:    timeStringAMPMDate							*/
//* Type:    Function									*/
//* Purpose: convert time of day in minutes to a zero-padded string	*/
//*		suitable for printing to the form text fields, and appends	*/
//*		the date.									*/
//* Arguments:										*/
//*   minutes : time of day in minutes						*/
//*   JD  : julian day									*/
//* Return value:										*/
//*   string of the format HH:MM[AM/PM] DDMon					*/
//***********************************************************************/

// timeStringAMPMDate returns a zero-padded string (HH:MM[AM/PM]) given time 
// in minutes and julian day, and appends the short date

	function timeStringAMPMDate(minutes, JD)
	{
		var julianday = JD;
		var floatHour = minutes / 60.0;
		var hour = Math.floor(floatHour);
		var floatMinute = 60.0 * (floatHour - Math.floor(floatHour));
		var minute = Math.floor(floatMinute);
		var floatSec = 60.0 * (floatMinute - Math.floor(floatMinute));
		var second = Math.floor(floatSec + 0.5);

		minute += (second >= 30)? 1 : 0;

		if (minute >= 60) 
		{
			minute -= 60;
			hour ++;
		}

		if (hour > 23) 
		{
			hour -= 24;
			julianday += 1.0;
		}

		if (hour < 0)
		{
			hour += 24;
			julianday -= 1.0;
		}

		var PM = false;
		if (hour > 12)
		{
			hour -= 12;
			PM = true;
		}

        if (hour == 12)
		{
            PM = true;
        }

		if (hour == 0)
		{
			PM = false;
			hour = 12;
		}

		var timeStr = hour + ":";
		if (minute < 10)	//	i.e. only one digit
			timeStr += "0" + minute + ((PM)?"PM":"AM");
		else
			timeStr += minute + ((PM)?"PM":"AM");

		return timeStr + " " + calcDayFromJD(julianday);
	}


//***********************************************************************/
//* Name:    timeStringDate								*/
//* Type:    Function									*/
//* Purpose: convert time of day in minutes to a zero-padded 24hr time	*/
//*		suitable for printing to the form text fields.  If time	*/
//*		crosses a day boundary, date is appended.				*/
//* Arguments:										*/
//*   minutes : time of day in minutes						*/
//*   JD  : julian day									*/
//* Return value:										*/
//*   string of the format HH:MM (DDMon)						*/
//***********************************************************************/

// timeStringDate returns a zero-padded string (HH:MM) given time in minutes
// and julian day, and appends the short date if time crosses a day boundary

	function timeStringDate(minutes, JD)
	{
		var julianday = JD;
		var floatHour = minutes / 60.0;
		var hour = Math.floor(floatHour);
		var floatMinute = 60.0 * (floatHour - Math.floor(floatHour));
		var minute = Math.floor(floatMinute);
		var floatSec = 60.0 * (floatMinute - Math.floor(floatMinute));
		var second = Math.floor(floatSec + 0.5);

		minute += (second >= 30)? 1 : 0;

		if (minute >= 60) 
		{
			minute -= 60;
			hour ++;
		}

		var daychange = false;
		if (hour > 23) 
		{
			hour -= 24;
			julianday += 1.0;
			daychange = true;
		}

		if (hour < 0)
		{
			hour += 24;
			julianday -= 1.0;
			daychange = true;
		}

		var timeStr = hour + ":";
		if (minute < 10)	//	i.e. only one digit
			timeStr += "0" + minute;
		else
			timeStr += minute;

		if (daychange) return timeStr + " " + calcDayFromJD(julianday);
		return timeStr;
	}

	
//***********************************************************************/
//* Name:    calcSun									*/
//* Type:    Main Function called by form controls				*/
//* Purpose: calculate time of sunrise and sunset for the entered date	*/
//*		and location.  In the special cases near earth's poles, 	*/
//*		the date of nearest sunrise and set are reported.		*/
//* Arguments:										*/
//*   riseSetForm : for displaying results					*/
//*   latLongForm : for reading latitude and longitude data			*/
//*   index : daylight saving yes/no select					*/
//*   index2 : city select index							*/
//* Return value:										*/
//*   none											*/
//*	(fills riseSetForm text fields with results of calculations)	*/
//***********************************************************************/

	function calcSun(riseSetForm, latLongForm, index, index2) 
	{
		if(index2 != 0)
		{
			setLatLong(latLongForm, index2);
		}

		var latitude = getLatitude(latLongForm);
		var longitude = getLongitude(latLongForm);
		var indexRS = riseSetForm.mos.selectedIndex
		if (isValidInput(riseSetForm, indexRS, latLongForm)) 
		{
			if((latitude >= -90) && (latitude < -89))
			{
				alert("All latitudes between 89 and 90 S\n will be set to -89");
				latLongForm["latDeg"].value = -89;
				latitude = -89;
			}
			if ((latitude <= 90) && (latitude > 89))
			{
				alert("All latitudes between 89 and 90 N\n will be set to 89");
				latLongForm["latDeg"].value = 89;
				latitude = 89;
			}
			
			//*****	Calculate the time of sunrise			

//*********************************************************************/
//****************   NEW STUFF   ******   January, 2001   ****************
//*********************************************************************/

			var JD = calcJD(parseFloat(riseSetForm["year"].value), indexRS + 1, parseFloat(riseSetForm["day"].value));
			var dow = calcDayOfWeek(JD);
			var doy = calcDayOfYear(indexRS + 1, parseFloat(riseSetForm["day"].value), IsLeapYear(riseSetForm["year"].value));
			var T = calcTimeJulianCent(JD);

			var alpha = calcSunRtAscension(T);
			var theta = calcSunDeclination(T);
			var Etime = calcEquationOfTime(T);

			//riseSetForm["dbug"].value = doy;

//*********************************************************************/

			var eqTime = Etime;
			var solarDec = theta;

			// Calculate sunrise for this date
			// if no sunrise is found, set flag nosunrise

			var nosunrise = false;

			var riseTimeGMT = calcSunriseUTC(JD, latitude, longitude);
			if (!isNumber(riseTimeGMT))
			{
				nosunrise = true;
			}

			// Calculate sunset for this date
			// if no sunset is found, set flag nosunset

			var nosunset = false;
			var setTimeGMT = calcSunsetUTC(JD, latitude, longitude);
			if (!isNumber(setTimeGMT))
			{
				nosunset = true;
			}

			var daySavings = YesNo[index].value;  // = 0 (no) or 60 (yes)
			var zone = latLongForm["hrsToGMT"].value;
			if(zone > 12 || zone < -12.5)
			{
				alert("The offset must be between -12.5 and 12.  \n Setting \"Off-Set\"=0");
				zone = "0";
				latLongForm["hrsToGMT"].value = zone;
			}

			if (!nosunrise)		// Sunrise was found
			{
				var riseTimeLST = riseTimeGMT - (60 * zone) + daySavings;	
					//	in minutes
				var riseStr = timeStringShortAMPM(riseTimeLST, JD);
				var utcRiseStr = timeStringDate(riseTimeGMT, JD);

				riseSetForm["sunrise"].value = riseStr;
				riseSetForm["utcsunrise"].value = utcRiseStr;
			}

			if (!nosunset)		// Sunset was found
			{
				var setTimeLST = setTimeGMT - (60 * zone) + daySavings;
				var setStr = timeStringShortAMPM(setTimeLST, JD);
				var utcSetStr = timeStringDate(setTimeGMT, JD);

				riseSetForm["sunset"].value = setStr;
				riseSetForm["utcsunset"].value = utcSetStr;
			}

			// Calculate solar noon for this date

			var solNoonGMT = calcSolNoonUTC(T, longitude);
			var solNoonLST = solNoonGMT - (60 * zone) + daySavings;

			var solnStr = timeString(solNoonLST);
			var utcSolnStr = timeString(solNoonGMT);

			riseSetForm["solnoon"].value = solnStr;
			riseSetForm["utcsolnoon"].value = utcSolnStr;

			var tsnoon = calcTimeJulianCent(calcJDFromJulianCent(T) -0.5 + solNoonGMT/1440.0); 

			eqTime = calcEquationOfTime(tsnoon);
			solarDec = calcSunDeclination(tsnoon);

			riseSetForm["eqTime"].value = (Math.floor(100*eqTime))/100;
			riseSetForm["solarDec"].value = (Math.floor(100*(solarDec)))/100;
			
			//***********Convert lat and long to standard format
			convLatLong(latLongForm);

			// report special cases of no sunrise

			if(nosunrise)
			{ 
				riseSetForm["utcsunrise"].value = "";
				// if Northern hemisphere and spring or summer, OR  
				// if Southern hemisphere and fall or winter, use 
				// previous sunrise and next sunset

				if ( ((latitude > 66.4) && (doy > 79) && (doy < 267)) ||
				   ((latitude < -66.4) && ((doy < 83) || (doy > 263))) )
				{
					newjd = findRecentSunrise(JD, latitude, longitude);
					newtime = calcSunriseUTC(newjd, latitude, longitude)
						 - (60 * zone) + daySavings;
					if (newtime > 1440)
					{
						newtime -= 1440;
						newjd += 1.0;
					}
					if (newtime < 0)
					{
						newtime += 1440;
						newjd -= 1.0;
					}
					riseSetForm["sunrise"].value = 
						timeStringAMPMDate(newtime, newjd);
					riseSetForm["utcsunrise"].value = "prior sunrise";
				}

				// if Northern hemisphere and fall or winter, OR 
				// if Southern hemisphere and spring or summer, use 
				// next sunrise and previous sunset

				else if ( ((latitude > 66.4) && ((doy < 83) || (doy > 263))) ||
					((latitude < -66.4) && (doy > 79) && (doy < 267)) )
				{
					newjd = findNextSunrise(JD, latitude, longitude);
					newtime = calcSunriseUTC(newjd, latitude, longitude)
						 - (60 * zone) + daySavings;
					if (newtime > 1440)
					{
						newtime -= 1440;
						newjd += 1.0;
					}
					if (newtime < 0)
					{
						newtime += 1440;
						newjd -= 1.0;
					}
					riseSetForm["sunrise"].value = 
						timeStringAMPMDate(newtime, newjd);
//					riseSetForm["sunrise"].value = calcDayFromJD(newjd)
//						+ " " + timeStringDate(newtime, newjd);
					riseSetForm["utcsunrise"].value = "next sunrise";
				}
				else 
				{
					alert("Cannot Find Sunrise!");
				}

				// alert("Last Sunrise was on day " + findRecentSunrise(JD, latitude, longitude));
				// alert("Next Sunrise will be on day " + findNextSunrise(JD, latitude, longitude));

			}

			if(nosunset)
			{ 
				riseSetForm["utcsunset"].value = "";
				// if Northern hemisphere and spring or summer, OR
				// if Southern hemisphere and fall or winter, use 
				// previous sunrise and next sunset

				if ( ((latitude > 66.4) && (doy > 79) && (doy < 267)) ||
				   ((latitude < -66.4) && ((doy < 83) || (doy > 263))) )
				{
					newjd = findNextSunset(JD, latitude, longitude);
					newtime = calcSunsetUTC(newjd, latitude, longitude)
						 - (60 * zone) + daySavings;
					if (newtime > 1440)
					{
						newtime -= 1440;
						newjd += 1.0;
					}
					if (newtime < 0)
					{
						newtime += 1440;
						newjd -= 1.0;
					}
					riseSetForm["sunset"].value = 
						timeStringAMPMDate(newtime, newjd);
					riseSetForm["utcsunset"].value = "next sunset";
					riseSetForm["utcsolnoon"].value = "";
				}

				// if Northern hemisphere and fall or winter, OR
				// if Southern hemisphere and spring or summer, use 
				// next sunrise and last sunset

				else if ( ((latitude > 66.4) && ((doy < 83) || (doy > 263))) ||
					((latitude < -66.4) && (doy > 79) && (doy < 267)) )
				{
					newjd = findRecentSunset(JD, latitude, longitude);
					newtime = calcSunsetUTC(newjd, latitude, longitude)
						 - (60 * zone) + daySavings;
					if (newtime > 1440)
					{
						newtime -= 1440;
						newjd += 1.0;
					}
					if (newtime < 0)
					{
						newtime += 1440;
						newjd -= 1.0;
					}
					riseSetForm["sunset"].value = 
						timeStringAMPMDate(newtime, newjd);
					riseSetForm["utcsunset"].value = "prior sunset";
					riseSetForm["solnoon"].value = "N/A";
					riseSetForm["utcsolnoon"].value = "";
				}

				else 
				{
					alert ("Cannot Find Sunset!");
				}
			}
		}
	}



//*********************************************************************/



</SCRIPT>

<FORM NAME="cityLatLong">
	<CENTER><TABLE BORDER>
	<TR>
		<TD ALIGN="CENTER"><H5>City:</TD>
		<TD></TD>
		<TD ALIGN="CENTER"><H5>Deg:</TD>

		<TD ALIGN="CENTER"><H5>Min:</TD>

		<TD ALIGN="CENTER"><H5>Sec:</TD>
		<TD ALIGN="CENTER" COLSPAN="2"><H5><a href="./timezone.html">Time Zone</TD>
</TR>

<TR>
<TD ALIGN="TOP">
<CENTER>
<SELECT NAME="cities" onChange="calcSun(riseSetCalc, cityLatLong, cityLatLong.dayAns.selectedIndex, cityLatLong.cities.selectedIndex);">

	    		<SCRIPT LANGUAGE="JavaScript">
	    		for (i = 0; i < City.length; i++) {
				if(City[i].name == "Boulder, CO")
				{
				document.writeln("<OPTION SELECTED>" + City[i].name);
	    		}
				else
				document.writeln("<OPTION>" + City[i].name);
				} 

				</SCRIPT>

	  		</SELECT>
</CENTER>
</TD>
		<TD ALIGN="CENTER"><H5><a href="./glossary.html#latitude">Lat</a>:<BR>North=+<BR>South=<FONT SIZE = 4>-</FONT></TD>

		<TD><INPUT TYPE="text" NAME="latDeg" VALUE="40" SIZE="4"></TD>

		<TD><INPUT TYPE="text" NAME="latMin" VALUE="7" SIZE="4"></TD>

		<TD><INPUT TYPE="text" NAME="latSec" VALUE="30" SIZE="4"></TD>
		<TD ROWSPAN=2 ALIGN="CENTER"><H5>Offset<BR>to 
			<a href="./glossary.html#UTC">UTC</a><BR>(MST=+7):<BR></H5>
		<INPUT TYPE="text" NAME="hrsToGMT" VALUE="7" SIZE="5"></TD>
		<TD ROWSPAN=2 ALIGN = "CENTER"><H5><a href="./glossary.html#daylightsavingtime">Daylight<BR>Saving<BR>Time</a>:<BR></H5>
		<SELECT NAME="dayAns" onChange="calcSun(riseSetCalc, cityLatLong, cityLatLong.dayAns.selectedIndex, cityLatLong.cities.selectedIndex);">
	<SCRIPT LANGUAGE="JavaScript">
		for(i=0; i < YesNo.length; i++)
		{
			document.writeln("<OPTION>" + YesNo[i].daySave);
		}
	</SCRIPT>
		</SELECT>

		</TD>
	</TR>
<TR>
 		<TD ALIGN="CENTER"><H5><a href="./sollinks.html#latlong">Click here for help finding<br>
			your lat/long coordinates</a>
		</TD>


		<TD ALIGN="CENTER"><H5><a href="./glossary.html#longitude">Long</a>:<BR>West=+<BR>East=<FONT SIZE = 4>-</FONT></TD>

		<TD><INPUT TYPE="text" NAME="lonDeg" VALUE="105" SIZE="4"></TD>

		<TD><INPUT TYPE="text" NAME="lonMin" VALUE="14" SIZE="4"></TD>

		<TD><INPUT TYPE="text" NAME="lonSec" VALUE="13" SIZE="4"></TD>

	</TR>
	<TR>
	<TD COLSPAN=7>
<b>Note:</b>  To manually enter latitude/longitude, select <b>Enter 
Lat/Long -></b> from the<br>City pulldown menu, and enter the
values in the text boxes to the right.
	</TD></TR>

	</TABLE>
</CENTER>
</FORM>



<FORM NAME="riseSetCalc">
	<CENTER><TABLE BORDER>

	<TR>

	  	<TD ALIGN="CENTER"><H5>Month:</TD>

		<TD ALIGN="CENTER"><H5>Day:</TD>

		<TD ALIGN="CENTER"><H5>Year (e.g. 2000):</TD>




	</TR>

	<TR>

	  	<TD>

			<SELECT NAME="mos" onChange="calcSun(riseSetCalc, cityLatLong, cityLatLong.dayAns.selectedIndex, cityLatLong.cities.selectedIndex);">

  	<SCRIPT LANGUAGE="JavaScript">
		dateObj1 = new Date();
		thismonth = dateObj1.getMonth();
		today = dateObj1.getDate(); 
		for (i = 0; i < monthList.length; i++) 
		{
			if (i == thismonth) 
			{
				document.writeln("<OPTION SELECTED>" + monthList[i].name);
			}
			else 
			{
				document.writeln("<OPTION>" + monthList[i].name);
			}
		}

	</SCRIPT>

	  		</SELECT>

	  	</TD>

	 <SCRIPT LANGUAGE="JavaScript">
		dateObj2 = new Date();
		thisday = dateObj2.getDate();
	  	
		document.writeln("<TD><CENTER><INPUT TYPE=\"text\" NAME=\"day\" SIZE=\"2\" VALUE=\"" + thisday + "\"></TD>");

		dateObj3 = new Date();
		thisYear = dateObj3.getYear();
		if(thisYear <=199)
		{
			thisYear=(thisYear*1)+1900;
		}
		document.writeln("<TD><CENTER><INPUT TYPE=\"text\" NAME=\"year\" SIZE=\"4\"VALUE=\"" + thisYear*1 + "\"></TD>");
	</SCRIPT>


	</TR>
</TABLE>

<br>

	  	<INPUT TYPE="button" NAME="RiseSet" VALUE="     Calculate Sunrise/Sunset     " 

			onClick="calcSun(riseSetCalc, cityLatLong, cityLatLong.dayAns.selectedIndex, cityLatLong.cities.selectedIndex);">

<br>

<BR>
<TABLE BORDER>
 	<TR>	
		<TD ALIGN="CENTER"><H5><a href="./glossary.html#equationoftime">Equation <BR>of Time</a><BR>(minutes):</TD>
		<TD ALIGN="CENTER"><H5><a href="./glossary.html#solardeclination">Solar<BR>Declination</a><BR> (degrees):</TD>
		<TD ALIGN="CENTER"><H5><a href="./glossary.html#apparentsunrise">Apparent<br>Sunrise</a>:</TD>
		<TD ALIGN="CENTER"><H5><a href="./glossary.html#solarnoon">Solar Noon</a>:</TD>
		<TD ALIGN="CENTER"><H5><a href="./glossary.html#apparentsunrise">Apparent<br>Sunset</a>:</TD>
		<TD ALIGN="CENTER"><H5>Time<br>Zone</TD>
	</TR>
	<TR>
		<TD><CENTER><INPUT TYPE="text" NAME="eqTime" SIZE = "8"></TD>
		<TD><CENTER><INPUT TYPE="text" NAME="solarDec" SIZE = "8"></TD>
		<TD><CENTER><INPUT TYPE="text" NAME="sunrise" SIZE="13"></TD>
		<TD><CENTER><INPUT TYPE="text" NAME="solnoon" SIZE="8"></TD>
		<TD><CENTER><INPUT TYPE="text" NAME="sunset" SIZE="13"></TD>
		<TD ALIGN="LEFT"><H5>Local</TD>

	</TR>
    <TR>
		<TD ALIGN="CENTER" COLSPAN="2">
		<!-- <CENTER><INPUT TYPE="text" NAME="dbug" SIZE="15"> -->
		</TD>
		<TD><CENTER><INPUT TYPE="text" NAME="utcsunrise" SIZE="13"></TD>
		<TD><CENTER><INPUT TYPE="text" NAME="utcsolnoon" SIZE="8"></TD>
		<TD><CENTER><INPUT TYPE="text" NAME="utcsunset" SIZE="13"></TD>
		<TD ALIGN="CENTER"><H5>UTC</TD>
</CENTER></TR>
</TABLE>
</CENTER>
</FORM></FONT>

<HR>
<CENTER><P>
<B><H3>Directions:</b><BR><FONT SIZE=2>
</CENTER>
<OL>
<LI>
Select a location from the City pulldown menu, <U>OR</U> select 
<b>"Enter Lat/Long ->"</b> from the pulldown menu, and manually 
enter the latitude, longitude and time zone information in the 
appropriate text boxes.  The following sign conventions are used: 
</P></FONT>
<CENTER><TABLE BORDER>
<TR>
<TD ALIGN="CENTER"><B><FONT SIZE =2>Longitude:</B></TD>
<TD ALIGN="CENTER"><B><FONT SIZE =2>Latitude:</B></TD>
</TR>
<TR>
<TD ALIGN="CENTER"><FONT SIZE =2>Positive in Western Hemisphere</TD>
<TD ALIGN="CENTER"><FONT SIZE =2>Positive in Northern Hemisphere</TD>
</TR>
<TR>
<TD ALIGN="CENTER"><FONT SIZE =2>Negative in Eastern Hemisphere</TD>
<TD ALIGN="CENTER"><FONT SIZE =2>Negative in Southern Hemisphere</TD>
</TR>
</TABLE>
<P>
</CENTER><FONT SIZE=2>
Latitude and Longitude can be in deg/min/sec, or decimal degrees entered in 
the "Deg:" field.  If you select a city from the pulldown menu, the latitude, 
longitude and time zone fields will be filled in automatically.  If you want to input 
latitude, longitude or time zone manually, <u>be sure to select "Enter Lat/Long -->" 
from the City pulldown box, or your numbers will be overwritten</u> by the 
selected city's location.
</P>
</LI>
<LI>
<P>
You can enter a different time zone for a location by selecting 
"Enter Lat/Long -->" in the City pulldown box.  Otherwise, the time zone associated with
the selected city's Local Standard Time will be automatically entered.  
Selecting "Yes" in the Daylight Saving field will cause the resulting sunrise, 
sunset and solar noon times to be adjusted forward one hour.  If you are 
uncertain of the time zone for a location, refer to our 
<a href="./timezone.html">Time Zone Table</a>. 
</P>
</LI>
<LI>
<P>
The program retrieves the current date from your computer, and fills the 
month, day and year fields.  To perform calculations for a different date, 
simply select a month in the pull down menu, and enter the day and four digit 
year in the appropriate input boxes.  When entering a day or year, you will
need to click the "Calculate Sunrise/Sunset" button to update the results for
this date.
</P>
</LI>
<LI>
<P>
Once the calculation results are displayed, you may use your web browser's 
"Print" function to obtain a hardcopy of the results.
</P>
</LI>
<LI>
<P>
For locations above the Arctic Circle and below the Antarctic Circle, when
a sunrise or sunset does not occur on the given day the program locates the
local time and date of the 
most recent sunrise or sunset, and the next sunset or sunrise.  When this
occurs, the UTC fields will display "prior" or "next" information instead
of the UTC times.
</P>
</LI>
<LI>
<P>
NOTE: For latitudes greater than 72 degrees N and S, calculations are 
accurate to within 10 minutes.  For latitudes less than +/- 72&deg; accuracy
is approximately one minute.  See <a href="./calcdetails.html">Solar Calculation 
Details</a> for further explanation.
</P>
</LI>
</OL>

</FONT>
<BR>
<HR>

<h4>
<table border=0 width="100%">
<tr>
<td align="left">
<a href="./azel.html">Solar Position Calculator</a><br>
<a href="./calcdetails.html">Calculation Details</a><br>
<a href="./sollinks.html">Solar Calculator Links</a><br>
</td>

<td align="center">
<a href="http://www.noaa.gov/"><img src="./noaaemblemt.gif" border="0" alt="NOAA Emblem"></a>
</td>

<td align="right">
<a href="./timezone.html">Time Zone Table</a><br>
<a href="./glossary.html">Solar Calculator Glossary</a><br>
<a href="/index.html">Back to SRRB Homepage</a><br>
</td>
</tr>
</table>

</h4>

<center>by 
<a href="mailto:&#119;&#101;&#98;&#109;&#97;&#115;&#116;&#101;&#114;-&#115;&#114;&#114;&#98;&#46;gm&#100;&#64;n&#111;&#97;&#97;&#46;&#103;&#111;&#118;">Chris Cornwall</a>,
Aaron Horiuchi and Chris Lehman<br>
	<SCRIPT>
		document.write("Last Updated on " + document.lastModified + ".");
	</SCRIPT>
</center>

</BODY>
</HTML>

