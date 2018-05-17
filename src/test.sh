#!/usr/bin/env sh
./greedy ../{noche.dat,requests_1.dat} > test/gn1.csv;
./greedy ../{noche.dat,requests_2.dat} > test/gn2.csv;
./greedy ../{noche.dat,requests_3.dat} > test/gn3.csv;
./greedy ../{manana.dat,requests_1.dat} > test/gm1.csv;
./greedy ../{manana.dat,requests_2.dat} > test/gm2.csv;
./greedy ../{manana.dat,requests_3.dat} > test/gm3.csv;
./greedy ../{tarde.dat,requests_1.dat} > test/gt1.csv;
./greedy ../{tarde.dat,requests_2.dat} > test/gt2.csv;
./greedy ../{tarde.dat,requests_3.dat} > test/gt3.csv;

./harmonic ../{noche.dat,requests_1.dat} > test/hn1.csv;
./harmonic ../{noche.dat,requests_2.dat} > test/hn2.csv;
./harmonic ../{noche.dat,requests_3.dat} > test/hn3.csv;
./harmonic ../{manana.dat,requests_1.dat} > test/hm1.csv;
./harmonic ../{manana.dat,requests_2.dat} > test/hm2.csv;
./harmonic ../{manana.dat,requests_3.dat} > test/hm3.csv;
./harmonic ../{tarde.dat,requests_1.dat} > test/ht1.csv;
./harmonic ../{tarde.dat,requests_2.dat} > test/ht2.csv;
./harmonic ../{tarde.dat,requests_3.dat} > test/ht3.csv;
