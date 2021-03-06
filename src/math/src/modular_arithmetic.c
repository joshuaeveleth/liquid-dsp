/*
 * Copyright (c) 2007 - 2014 Joseph Gaeddert
 *
 * This file is part of liquid.
 *
 * liquid is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * liquid is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with liquid.  If not, see <http://www.gnu.org/licenses/>.
 */

//
// Modular arithmetic and related integer mathematics
//

#include <stdio.h>
#include <stdlib.h>

#include "liquid.h"

// determine if number is prime (slow, simple method)
int liquid_is_prime(unsigned int _n)
{
    if (_n < 4) return 1;

    unsigned int i;
    for (i=2; i<_n; i++) {
        if ( (_n % i) == 0)
            return 0;
    }

    return 1;
}

// compute number's prime factors
//  _n          :   number to factor
//  _factors    :   pre-allocated array of factors [size: LIQUID_MAX_FACTORS x 1]
//  _num_factors:   number of factors found, sorted ascending
void liquid_factor(unsigned int   _n,
                   unsigned int * _factors,
                   unsigned int * _num_factors)
{
    unsigned int k;
    unsigned int n = _n;
    unsigned int num_factors = 0;
    do {
        for (k=2; k<=n; k++) {
            if ( (n % k) == 0) {
                // k factors _n; append to list
                _factors[num_factors++] = k;
                n /= k;
                break;
            }
        }
    } while (n > 1 && num_factors < LIQUID_MAX_FACTORS);

    if (n > 1 && num_factors == LIQUID_MAX_FACTORS) {
        fprintf(stderr,"error, liquid_factor(), could not factor %u in %u numbers\n", _n, LIQUID_MAX_FACTORS);
        exit(1);
    }

    *_num_factors = num_factors;
}

// compute number's unique prime factors
//  _n          :   number to factor
//  _factors    :   pre-allocated array of factors [size: LIQUID_MAX_FACTORS x 1]
//  _num_factors:   number of unique factors found, sorted ascending
void liquid_unique_factor(unsigned int   _n,
                          unsigned int * _factors,
                          unsigned int * _num_factors)
{
    unsigned int k;
    unsigned int n = _n;
    unsigned int num_factors = 0;
    do {
        for (k=2; k<=n; k++) {
            if ( (n % k) == 0) {
                // k factors _n; append to list
                _factors[num_factors] = k;
                n /= k;
                
                if (num_factors == 0)
                    num_factors++;
                else if (_factors[num_factors-1] != k)
                    num_factors++;

                break;
            }
        }
    } while (n > 1 && num_factors < LIQUID_MAX_FACTORS);

    if (n > 1 && num_factors == LIQUID_MAX_FACTORS) {
        fprintf(stderr,"error, liquid_unqiue_factor(), could not factor %u in %u numbers\n", _n, LIQUID_MAX_FACTORS);
        exit(1);
    }

    *_num_factors = num_factors;
}

// find smallest primitive root of _n
unsigned int liquid_primitive_root(unsigned int _n)
{
    // TODO : flesh out this function
    return 1;
}

// find smallest primitive root of _n (assuming _n is prime)
unsigned int liquid_primitive_root_prime(unsigned int _n)
{
    // find unique factors of _n-1
    unsigned int unique_factors[LIQUID_MAX_FACTORS];
    unsigned int num_unique_factors = 0;
    unsigned int n = _n-1;
    unsigned int k;
    do {
        for (k=2; k<=n; k++) {
            if ( (n%k)==0 ) {
                // k is a factor of (_n-1)
                n /= k;

                // add element to end of table
                unique_factors[num_unique_factors] = k;

                // increment counter only if element is unique
                if (num_unique_factors == 0)
                    num_unique_factors++;
                else if (unique_factors[num_unique_factors-1] != k)
                    num_unique_factors++;
                break;
            }
        }
    } while (n > 1 && num_unique_factors < LIQUID_MAX_FACTORS);

#if 0
    // print unique factors
    printf("found %u unique factors of n-1 = %u\n", num_unique_factors, _n-1);
    for (k=0; k<num_unique_factors; k++)
        printf("  %3u\n", unique_factors[k]);
#endif

    // search for minimum integer for which
    //   g^( (_n-1)/m ) != 1 (mod _n)
    // for all unique roots 'm'
    unsigned int g;
    for (g=2; g<_n; g++) {
        int is_root = 1;
        for (k=0; k<num_unique_factors; k++) {
            unsigned int e = (_n-1) / unique_factors[k];
            if ( liquid_modpow(g,e,_n) == 1 ) {
                // not a root
                is_root = 0;
                break;
            }
        }

        if (is_root) {
            //printf("  %u is a primitive root!\n", g);
            break;
        }
    }

    return g;
}

// compute c = base^exp (mod n)
unsigned int liquid_modpow(unsigned int _base,
                           unsigned int _exp,
                           unsigned int _n)
{
    unsigned int c = 1;
    unsigned int i;
    for (i=0; i<_exp; i++)
        c = (c * _base) % _n;

    return c;
}

// Euler's totient function
unsigned int liquid_totient(unsigned int _n)
{
    // find unique prime factors
    unsigned int t = _n;    // totient
    unsigned int n = _n;
    unsigned int p = 0;     // last unique prime factor
    unsigned int k;
    do {
        for (k=2; k<=n; k++) {
            if ( (n%k)==0 ) {
                n /= k;

                if (p != k) {
                    // factor is unique
                    t *= k-1;
                    t /= k;
                }

                p = k;
                break;
            }
        }
    } while (n > 1);

    return t;
}

