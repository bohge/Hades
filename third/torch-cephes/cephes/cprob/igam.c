/*							igam.c
 *
 *	Incomplete gamma integral
 *
 *
 *
 * SYNOPSIS:
 *
 * double a, x, y, igam();
 *
 * y = igam( a, x );
 *
 * DESCRIPTION:
 *
 * The function is defined by
 *
 *                           x
 *                            -
 *                   1       | |  -t  a-1
 *  igam(a,x)  =   -----     |   e   t   dt.
 *                  -      | |
 *                 | (a)    -
 *                           0
 *
 *
 * In this implementation both arguments must be positive.
 * The integral is evaluated by either a power series or
 * continued fraction expansion, depending on the relative
 * values of a and x.
 *
 * ACCURACY:
 *
 *                      Relative error:
 * arithmetic   domain     # trials      peak         rms
 *    IEEE      0,30       200000       3.6e-14     2.9e-15
 *    IEEE      0,100      300000       9.9e-14     1.5e-14
 */
/*							igamc()
 *
 *	Complemented incomplete gamma integral
 *
 *
 *
 * SYNOPSIS:
 *
 * double a, x, y, igamc();
 *
 * y = igamc( a, x );
 *
 * DESCRIPTION:
 *
 * The function is defined by
 *
 *
 *  igamc(a,x)   =   1 - igam(a,x)
 *
 *                            inf.
 *                              -
 *                     1       | |  -t  a-1
 *               =   -----     |   e   t   dt.
 *                    -      | |
 *                   | (a)    -
 *                             x
 *
 *
 * In this implementation both arguments must be positive.
 * The integral is evaluated by either a power series or
 * continued fraction expansion, depending on the relative
 * values of a and x.
 *
 * ACCURACY:
 *
 * Tested at random a, x.
 *                a         x                      Relative error:
 * arithmetic   domain   domain     # trials      peak         rms
 *    IEEE     0.5,100   0,100      200000       1.9e-14     1.7e-15
 *    IEEE     0.01,0.5  0,100      200000       1.4e-13     1.6e-15
 */

/*
Cephes Math Library Release 2.8:  June, 2000
Copyright 1985, 1987, 2000 by Stephen L. Moshier
*/

#include "mconf.h"
#ifdef ANSIPROT
extern double torch_cephes_lgam ( double );
extern double torch_cephes_exp ( double );
extern double torch_cephes_log ( double );
extern double torch_cephes_fabs ( double );
extern double torch_cephes_igam ( double, double );
extern double torch_cephes_igamc ( double, double );
#else
double torch_cephes_lgam(), torch_cephes_exp(), torch_cephes_log(),
    torch_cephes_fabs(), torch_cephes_igam(), torch_cephes_igamc();
#endif

extern double torch_cephes_MACHEP, torch_cephes_MAXLOG, torch_cephes_NAN;
static double big = 4.503599627370496e15;
static double biginv =  2.22044604925031308085e-16;

double torch_cephes_igamc( a, x )
double a, x;
{
double ans, ax, c, yc, r, t, y, z;
double pk, pkm1, pkm2, qk, qkm1, qkm2;

if( (x < 0) || ( a <= 0) )
    {
    torch_cephes_mtherr("igamc", DOMAIN);
	return( torch_cephes_NAN );
    }

if( (x < 1.0) || (x < a) )
	return( 1.0 - torch_cephes_igam(a,x) );

ax = a * torch_cephes_log(x) - x - torch_cephes_lgam(a);
if( ax < -torch_cephes_MAXLOG )
	{
	torch_cephes_mtherr( "igamc", UNDERFLOW );
	return( 0.0 );
	}
ax = torch_cephes_exp(ax);

/* continued fraction */
y = 1.0 - a;
z = x + y + 1.0;
c = 0.0;
pkm2 = 1.0;
qkm2 = x;
pkm1 = x + 1.0;
qkm1 = z * x;
ans = pkm1/qkm1;

do
	{
	c += 1.0;
	y += 1.0;
	z += 2.0;
	yc = y * c;
	pk = pkm1 * z  -  pkm2 * yc;
	qk = qkm1 * z  -  qkm2 * yc;
	if( qk != 0 )
		{
		r = pk/qk;
		t = torch_cephes_fabs( (ans - r)/r );
		ans = r;
		}
	else
		t = 1.0;
	pkm2 = pkm1;
	pkm1 = pk;
	qkm2 = qkm1;
	qkm1 = qk;
	if( torch_cephes_fabs(pk) > big )
		{
		pkm2 *= biginv;
		pkm1 *= biginv;
		qkm2 *= biginv;
		qkm1 *= biginv;
		}
	}
while( t > torch_cephes_MACHEP );

return( ans * ax );
}



/* left tail of incomplete gamma function:
 *
 *          inf.      k
 *   a  -x   -       x
 *  x  e     >   ----------
 *           -     -
 *          k=0   | (a+k+1)
 *
 */

double torch_cephes_igam( a, x )
double a, x;
{
double ans, ax, c, r;

/* Check zero integration limit first */
if( x == 0 )
    return ( 0.0 );

if( (x < 0) || ( a <= 0) )
    {
    torch_cephes_mtherr("igam", DOMAIN);
	return( torch_cephes_NAN );
    }

if( (x > 1.0) && (x > a ) )
	return( 1.0 - torch_cephes_igamc(a,x) );

/* Compute  x**a * exp(-x) / gamma(a)  */
ax = a * torch_cephes_log(x) - x - torch_cephes_lgam(a);
if( ax < -torch_cephes_MAXLOG )
	{
	torch_cephes_mtherr( "igam", UNDERFLOW );
	return( 0.0 );
	}
ax = torch_cephes_exp(ax);

/* power series */
r = a;
c = 1.0;
ans = 1.0;

do
	{
	r += 1.0;
	c *= x/r;
	ans += c;
	}
while( c/ans > torch_cephes_MACHEP );

return( ans * ax/a );
}