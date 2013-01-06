/*
    <one line to give the library's name and an idea of what it does.>
    Copyright (C) 2012  Matteo De Carlo <email>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/


#include "randomservice.h"

#include <ctime>

RandomService randomService(time(0));

RandomService::RandomService(uint seed)
//    : gen(std::random_device)
{
    qsrand(seed);
    deviateAvailable = false;

    //TODO run a mini-benchmark to see witch of the two gaussian functions is faster
}


RandomService::~RandomService()
{

}

uint RandomService::randomInt()
{
    return qrand();
}

smReal RandomService::randomReal()
{
    smReal rand = qrand();
    rand /= RandMax;
    return rand;
}

/******************************************************************************/
/* randn()
 *
 * Normally (Gaussian) distributed random numbers, using the Box-Muller
 * transformation.  This transformation takes two uniformly distributed deviates
 * within the unit circle, and transforms them into two independently
 * distributed normal deviates.  Utilizes the internal rand() function; this can
 * easily be changed to use a better and faster RNG.
 *
 * The parameters passed to the function are the mean and standard deviation of
 * the desired distribution.  The default values used, when no arguments are
 * passed, are 0 and 1 - the standard normal distribution.
 *
 *
 * Two functions are provided:
 *
 * The first uses the so-called polar version of the B-M transformation, using
 * multiple calls to a uniform RNG to ensure the initial deviates are within the
 * unit circle.  This avoids making any costly trigonometric function calls.
 *
 * The second makes only a single set of calls to the RNG, and calculates a
 * position within the unit circle with two trigonometric function calls.
 *
 * The polar version is generally superior in terms of speed; however, on some
 * systems, the optimization of the math libraries may result in better
 * performance of the second.  Try it out on the target system to see which
 * works best for you.  On my test machine (Athlon 3800+), the non-trig version
 * runs at about 3x10^6 calls/s; while the trig version runs at about
 * 1.8x10^6 calls/s (-O2 optimization).
 *
 *
 * Example calls:
 * randn_notrig();	//returns normal deviate with mean=0.0, std. deviation=1.0
 * randn_notrig(5.2,3.0);	//returns deviate with mean=5.2, std. deviation=3.0
 *
 *
 * Dependencies - requires <cmath> for the sqrt(), sin(), and cos() calls, and a
 * #defined value for PI.
 *
 * snippet from http://www.dreamincode.net/code/snippet1446.htm
 */

/******************************************************************************/
//	"Polar" version without trigonometric calls
smReal RandomService::randn_notrig(smReal mu, smReal sigma) {
// 	static bool deviateAvailable=false;	//	flag
// 	static float storedDeviate;			//	deviate from previous calculation
	smReal polar, rsquared, var1, var2;

	//	If no deviate has been stored, the polar Box-Muller transformation is
	//	performed, producing two independent normally-distributed random
	//	deviates.  One is stored for the next round, and one is returned.
	if (!deviateAvailable) {

		//	choose pairs of uniformly distributed deviates, discarding those
		//	that don't fall within the unit circle
		do {
			var1=2.0*( smReal(rand())/smReal(RAND_MAX) ) - 1.0;
			var2=2.0*( smReal(rand())/smReal(RAND_MAX) ) - 1.0;
			rsquared=var1*var1+var2*var2;
		} while ( rsquared>=1.0 || rsquared == 0.0);

		//	calculate polar tranformation for each deviate
		polar=sqrt(-2.0*log(rsquared)/rsquared);

		//	store first deviate and set flag
		storedDeviate=var1*polar;
		deviateAvailable=true;

		//	return second deviate
		return var2*polar*sigma + mu;
	}

	//	If a deviate is available from a previous call to this function, it is
	//	returned, and the flag is set to false.
	else {
		deviateAvailable=false;
		return storedDeviate*sigma + mu;
	}
}


/******************************************************************************/
//	Standard version with trigonometric calls
#define PI 3.14159265358979323846

smReal RandomService::randn_trig(smReal mu, smReal sigma) {
// 	static bool deviateAvailable=false;	//	flag
// 	static float storedDeviate;			//	deviate from previous calculation
	smReal dist, angle;

	//	If no deviate has been stored, the standard Box-Muller transformation is
	//	performed, producing two independent normally-distributed random
	//	deviates.  One is stored for the next round, and one is returned.
	if (!deviateAvailable) {

		//	choose a pair of uniformly distributed deviates, one for the
		//	distance and one for the angle, and perform transformations
		dist=sqrt( -2.0 * log(smReal(rand()) / smReal(RAND_MAX)) );
		angle=2.0 * PI * (smReal(rand()) / smReal(RAND_MAX));

		//	calculate and store first deviate and set flag
		storedDeviate=dist*cos(angle);
		deviateAvailable=true;

		//	calcaulate return second deviate
		return dist * sin(angle) * sigma + mu;
	}

	//	If a deviate is available from a previous call to this function, it is
	//	returned, and the flag is set to false.
	else {
		deviateAvailable=false;
		return storedDeviate*sigma + mu;
	}
}

smReal RandomService::randomNormal(smReal mu, smReal sigma)
{   
    return this->randn_notrig(mu,sigma);
}


