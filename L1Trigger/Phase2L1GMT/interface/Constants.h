#ifndef PHASE2GMT_CONSTANTS
#define PHASE2GMT_CONSTANTS

#include "ap_int.h"

namespace Phase2L1GMT {

  const int BITSCURV             = 15; 
  const int BITSPT               = 13; 
  const int BITSPHI              = 13;
  const int BITSETA              = 13;
  const int BITSZ0               = 10;
  const int BITSD0               = 12;
  const int BITSQUALITY          = 4; 

  const int BITSTTCURV           = 15;
  const int BITSTTPHI            = 12;
  const int BITSTTTANL           = 16;
  const int BITSTTZ0             = 12;
  const int BITSTTD0             = 13;
  const int BITSTTCHI2           = 4;
  const int BITSTTBENDCHI2       = 3;
  const int BITSTTMASK           = 7;
  const int BITSTTTRACKMVA       = 3;
  const int BITSTTOTHERMVA       = 6;

  const float maxCurv_ = 0.00855;  // 2 GeV pT Rinv is in cm
  const float maxPhi_ = 1.026;     // relative to the center of the sector
  const float maxTanl_ = 8.0;
  const float maxZ0_ = 30.;
  const float maxD0_ = 15.4;




const   ap_uint<BITSPT> ptLUT[2048]={8191,8191,8191,8191,8191,8191,8191,8191,8191,8191,8191,8191,8191,7801,7281,6826,6425,6068,5748,5461,5201,4964,4748,4551,4369,4201,4045,3900,3766,3640,3523,3413,3309,3212,3120,3034,2952,2874,2800,2730,2664,2600,2540,2482,2427,2374,2323,2275,2229,2184,2141,2100,2060,2022,1985,1950,1916,1883,1851,1820,1790,1761,1733,1706,1680,1654,1630,1606,1582,1560,1538,1517,1496,1476,1456,1437,1418,1400,1382,1365,1348,1332,1315,1300,1285,1270,1255,1241,1227,1213,1200,1187,1174,1161,1149,1137,1126,1114,1103,1092,1081,1070,1060,1050,1040,1030,1020,1011,1002,992,984,975,966,958,949,941,933,925,917,910,902,895,888,880,873,866,860,853,846,840,833,827,821,815,809,803,797,791,785,780,774,769,763,758,753,748,743,738,733,728,723,718,713,709,704,700,695,691,686,682,678,674,670,666,661,657,654,650,646,642,638,635,631,627,624,620,617,613,610,606,603,600,596,593,590,587,584,580,577,574,571,568,565,563,560,557,554,551,548,546,543,540,538,535,532,530,527,525,522,520,517,515,512,510,508,505,503,501,498,496,494,492,489,487,485,483,481,479,476,474,472,470,468,466,464,462,460,458,457,455,453,451,449,447,445,444,442,440,438,436,435,433,431,430,428,426,425,423,421,420,418,416,415,413,412,410,409,407,406,404,403,401,400,398,397,395,394,392,391,390,388,387,385,384,383,381,380,379,377,376,375,374,372,371,370,369,367,366,365,364,362,361,360,359,358,356,355,354,353,352,351,350,348,347,346,345,344,343,342,341,340,339,338,337,336,335,334,333,331,330,329,328,328,327,326,325,324,323,322,321,320,319,318,317,316,315,314,313,312,312,311,310,309,308,307,306,305,305,304,303,302,301,300,300,299,298,297,296,296,295,294,293,292,292,291,290,289,288,288,287,286,285,285,284,283,282,282,281,280,280,279,278,277,277,276,275,275,274,273,273,272,271,271,270,269,269,268,267,267,266,265,265,264,263,263,262,261,261,260,260,259,258,258,257,257,256,255,255,254,254,253,252,252,251,251,250,249,249,248,248,247,247,246,246,245,244,244,243,243,242,242,241,241,240,240,239,239,238,237,237,236,236,235,235,234,234,233,233,232,232,231,231,230,230,229,229,228,228,228,227,227,226,226,225,225,224,224,223,223,222,222,222,221,221,220,220,219,219,218,218,218,217,217,216,216,215,215,215,214,214,213,213,212,212,212,211,211,210,210,210,209,209,208,208,208,207,207,206,206,206,205,205,204,204,204,203,203,203,202,202,201,201,201,200,200,200,199,199,198,198,198,197,197,197,196,196,196,195,195,195,194,194,194,193,193,192,192,192,191,191,191,190,190,190,189,189,189,188,188,188,187,187,187,187,186,186,186,185,185,185,184,184,184,183,183,183,182,182,182,182,181,181,181,180,180,180,179,179,179,179,178,178,178,177,177,177,177,176,176,176,175,175,175,175,174,174,174,173,173,173,173,172,172,172,172,171,171,171,170,170,170,170,169,169,169,169,168,168,168,168,167,167,167,167,166,166,166,165,165,165,165,164,164,164,164,164,163,163,163,163,162,162,162,162,161,161,161,161,160,160,160,160,159,159,159,159,158,158,158,158,158,157,157,157,157,156,156,156,156,156,155,155,155,155,154,154,154,154,154,153,153,153,153,152,152,152,152,152,151,151,151,151,151,150,150,150,150,150,149,149,149,149,149,148,148,148,148,148,147,147,147,147,147,146,146,146,146,146,145,145,145,145,145,144,144,144,144,144,143,143,143,143,143,142,142,142,142,142,142,141,141,141,141,141,140,140,140,140,140,140,139,139,139,139,139,138,138,138,138,138,138,137,137,137,137,137,137,136,136,136,136,136,136,135,135,135,135,135,135,134,134,134,134,134,134,133,133,133,133,133,133,132,132,132,132,132,132,131,131,131,131,131,131,130,130,130,130,130,130,130,129,129,129,129,129,129,128,128,128,128,128,128,128,127,127,127,127,127,127,127,126,126,126,126,126,126,125,125,125,125,125,125,125,124,124,124,124,124,124,124,123,123,123,123,123,123,123,123,122,122,122,122,122,122,122,121,121,121,121,121,121,121,120,120,120,120,120,120,120,120,119,119,119,119,119,119,119,118,118,118,118,118,118,118,118,117,117,117,117,117,117,117,117,116,116,116,116,116,116,116,116,115,115,115,115,115,115,115,115,114,114,114,114,114,114,114,114,114,113,113,113,113,113,113,113,113,112,112,112,112,112,112,112,112,112,111,111,111,111,111,111,111,111,111,110,110,110,110,110,110,110,110,109,109,109,109,109,109,109,109,109,109,108,108,108,108,108,108,108,108,108,107,107,107,107,107,107,107,107,107,106,106,106,106,106,106,106,106,106,106,105,105,105,105,105,105,105,105,105,105,104,104,104,104,104,104,104,104,104,104,103,103,103,103,103,103,103,103,103,103,102,102,102,102,102,102,102,102,102,102,101,101,101,101,101,101,101,101,101,101,101,100,100,100,100,100,100,100,100,100,100,100,99,99,99,99,99,99,99,99,99,99,99,98,98,98,98,98,98,98,98,98,98,98,97,97,97,97,97,97,97,97,97,97,97,97,96,96,96,96,96,96,96,96,96,96,96,95,95,95,95,95,95,95,95,95,95,95,95,94,94,94,94,94,94,94,94,94,94,94,94,93,93,93,93,93,93,93,93,93,93,93,93,93,92,92,92,92,92,92,92,92,92,92,92,92,92,91,91,91,91,91,91,91,91,91,91,91,91,91,90,90,90,90,90,90,90,90,90,90,90,90,90,89,89,89,89,89,89,89,89,89,89,89,89,89,89,88,88,88,88,88,88,88,88,88,88,88,88,88,88,87,87,87,87,87,87,87,87,87,87,87,87,87,87,86,86,86,86,86,86,86,86,86,86,86,86,86,86,86,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,84,84,84,84,84,84,84,84,84,84,84,84,84,84,84,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,81,81,81,81,81,81,81,81,81,81,81,81,81,81,81,81,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,79,79,79,79,79,79,79,79,79,79,79,79,79,79,79,79,79,78,78,78,78,78,78,78,78,78,78,78,78,78,78,78,78,78,78,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,76,76,76,76,76,76,76,76,76,76,76,76,76,76,76,76,76,76,76,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,74,74,74,74,74,74,74,74,74,74,74,74,74,74,74,74,74,74,74,74,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,72,72,72,72,72,72,72,72,72,72,72,72,72,72,72,72,72,72,72,72,72,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,70,70,70,70,70,70,70,70,70,70,70,70,70,70,70,70,70,70,70,70,70,70,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,68,68,68,68,68,68,68,68,68,68,68,68,68,68,68,68,68,68,68,68,68,68,68,68,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,61,61,61,61,61,61,61,61,61,61,61,61,61,61,61,61,61,61,61,61,61,61,61,61,61,61,61,61,61,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,58,58,58,58,58,58,58,58,58,58,58,58,58,58,58,58,58,58,58,58,58,58,58,58,58,58,58,58,58,58,58,58,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,55,55,55,55,55,55,55,55,55,55,55,55,55,55,55,55,55,55,55,55,55,55,55,55,55,55,55,55,55,55,55,55,55,55,55,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53};


ap_uint<BITSETA-1> etaLUT[2048]={0,5,10,15,20,25,30,35,40,45,50,56,61,66,71,76,81,86,91,96,101,106,111,116,122,127,132,137,142,147,152,157,162,167,172,177,182,187,192,197,202,207,212,217,223,228,233,238,243,248,253,258,263,268,273,278,282,287,292,297,302,307,312,317,322,327,332,337,342,347,352,357,362,366,371,376,381,386,391,396,401,405,410,415,420,425,430,434,439,444,449,454,459,463,468,473,478,482,487,492,497,501,506,511,516,520,525,530,534,539,544,548,553,558,562,567,572,576,581,586,590,595,599,604,609,613,618,622,627,631,636,641,645,650,654,659,663,668,672,677,681,686,690,694,699,703,708,712,717,721,725,730,734,739,743,747,752,756,760,765,769,773,778,782,786,790,795,799,803,807,812,816,820,824,829,833,837,841,845,850,854,858,862,866,870,875,879,883,887,891,895,899,903,907,911,915,919,923,928,932,936,940,944,948,952,956,960,963,967,971,975,979,983,987,991,995,999,1003,1007,1010,1014,1018,1022,1026,1030,1034,1037,1041,1045,1049,1053,1056,1060,1064,1068,1071,1075,1079,1083,1086,1090,1094,1098,1101,1105,1109,1112,1116,1120,1123,1127,1131,1134,1138,1141,1145,1149,1152,1156,1159,1163,1167,1170,1174,1177,1181,1184,1188,1191,1195,1198,1202,1205,1209,1212,1216,1219,1223,1226,1230,1233,1237,1240,1243,1247,1250,1254,1257,1260,1264,1267,1270,1274,1277,1281,1284,1287,1291,1294,1297,1300,1304,1307,1310,1314,1317,1320,1323,1327,1330,1333,1336,1340,1343,1346,1349,1353,1356,1359,1362,1365,1369,1372,1375,1378,1381,1384,1387,1391,1394,1397,1400,1403,1406,1409,1412,1415,1419,1422,1425,1428,1431,1434,1437,1440,1443,1446,1449,1452,1455,1458,1461,1464,1467,1470,1473,1476,1479,1482,1485,1488,1491,1494,1497,1500,1503,1506,1508,1511,1514,1517,1520,1523,1526,1529,1532,1534,1537,1540,1543,1546,1549,1552,1554,1557,1560,1563,1566,1568,1571,1574,1577,1580,1582,1585,1588,1591,1594,1596,1599,1602,1605,1607,1610,1613,1615,1618,1621,1624,1626,1629,1632,1634,1637,1640,1642,1645,1648,1650,1653,1656,1658,1661,1664,1666,1669,1672,1674,1677,1679,1682,1685,1687,1690,1692,1695,1698,1700,1703,1705,1708,1710,1713,1716,1718,1721,1723,1726,1728,1731,1733,1736,1738,1741,1743,1746,1748,1751,1753,1756,1758,1761,1763,1766,1768,1771,1773,1775,1778,1780,1783,1785,1788,1790,1792,1795,1797,1800,1802,1805,1807,1809,1812,1814,1817,1819,1821,1824,1826,1828,1831,1833,1835,1838,1840,1842,1845,1847,1849,1852,1854,1856,1859,1861,1863,1866,1868,1870,1873,1875,1877,1879,1882,1884,1886,1889,1891,1893,1895,1898,1900,1902,1904,1907,1909,1911,1913,1915,1918,1920,1922,1924,1927,1929,1931,1933,1935,1938,1940,1942,1944,1946,1948,1951,1953,1955,1957,1959,1961,1964,1966,1968,1970,1972,1974,1976,1979,1981,1983,1985,1987,1989,1991,1993,1996,1998,2000,2002,2004,2006,2008,2010,2012,2014,2016,2019,2021,2023,2025,2027,2029,2031,2033,2035,2037,2039,2041,2043,2045,2047,2049,2051,2053,2055,2057,2059,2061,2063,2065,2067,2069,2071,2073,2075,2077,2079,2081,2083,2085,2087,2089,2091,2093,2095,2097,2099,2101,2103,2105,2107,2109,2111,2113,2115,2117,2118,2120,2122,2124,2126,2128,2130,2132,2134,2136,2138,2140,2141,2143,2145,2147,2149,2151,2153,2155,2157,2158,2160,2162,2164,2166,2168,2170,2172,2173,2175,2177,2179,2181,2183,2184,2186,2188,2190,2192,2194,2195,2197,2199,2201,2203,2205,2206,2208,2210,2212,2214,2215,2217,2219,2221,2223,2224,2226,2228,2230,2232,2233,2235,2237,2239,2240,2242,2244,2246,2248,2249,2251,2253,2255,2256,2258,2260,2262,2263,2265,2267,2268,2270,2272,2274,2275,2277,2279,2281,2282,2284,2286,2287,2289,2291,2293,2294,2296,2298,2299,2301,2303,2304,2306,2308,2309,2311,2313,2315,2316,2318,2320,2321,2323,2325,2326,2328,2330,2331,2333,2334,2336,2338,2339,2341,2343,2344,2346,2348,2349,2351,2353,2354,2356,2357,2359,2361,2362,2364,2366,2367,2369,2370,2372,2374,2375,2377,2378,2380,2382,2383,2385,2386,2388,2390,2391,2393,2394,2396,2397,2399,2401,2402,2404,2405,2407,2409,2410,2412,2413,2415,2416,2418,2419,2421,2423,2424,2426,2427,2429,2430,2432,2433,2435,2436,2438,2439,2441,2443,2444,2446,2447,2449,2450,2452,2453,2455,2456,2458,2459,2461,2462,2464,2465,2467,2468,2470,2471,2473,2474,2476,2477,2479,2480,2482,2483,2485,2486,2488,2489,2491,2492,2494,2495,2496,2498,2499,2501,2502,2504,2505,2507,2508,2510,2511,2513,2514,2515,2517,2518,2520,2521,2523,2524,2526,2527,2528,2530,2531,2533,2534,2536,2537,2538,2540,2541,2543,2544,2546,2547,2548,2550,2551,2553,2554,2555,2557,2558,2560,2561,2562,2564,2565,2567,2568,2569,2571,2572,2574,2575,2576,2578,2579,2580,2582,2583,2585,2586,2587,2589,2590,2591,2593,2594,2596,2597,2598,2600,2601,2602,2604,2605,2606,2608,2609,2611,2612,2613,2615,2616,2617,2619,2620,2621,2623,2624,2625,2627,2628,2629,2631,2632,2633,2635,2636,2637,2639,2640,2641,2643,2644,2645,2646,2648,2649,2650,2652,2653,2654,2656,2657,2658,2660,2661,2662,2663,2665,2666,2667,2669,2670,2671,2673,2674,2675,2676,2678,2679,2680,2682,2683,2684,2685,2687,2688,2689,2690,2692,2693,2694,2696,2697,2698,2699,2701,2702,2703,2704,2706,2707,2708,2709,2711,2712,2713,2714,2716,2717,2718,2719,2721,2722,2723,2724,2726,2727,2728,2729,2731,2732,2733,2734,2736,2737,2738,2739,2740,2742,2743,2744,2745,2747,2748,2749,2750,2751,2753,2754,2755,2756,2757,2759,2760,2761,2762,2764,2765,2766,2767,2768,2770,2771,2772,2773,2774,2776,2777,2778,2779,2780,2781,2783,2784,2785,2786,2787,2789,2790,2791,2792,2793,2795,2796,2797,2798,2799,2800,2802,2803,2804,2805,2806,2807,2809,2810,2811,2812,2813,2814,2816,2817,2818,2819,2820,2821,2822,2824,2825,2826,2827,2828,2829,2831,2832,2833,2834,2835,2836,2837,2839,2840,2841,2842,2843,2844,2845,2846,2848,2849,2850,2851,2852,2853,2854,2856,2857,2858,2859,2860,2861,2862,2863,2864,2866,2867,2868,2869,2870,2871,2872,2873,2875,2876,2877,2878,2879,2880,2881,2882,2883,2884,2886,2887,2888,2889,2890,2891,2892,2893,2894,2895,2897,2898,2899,2900,2901,2902,2903,2904,2905,2906,2907,2908,2910,2911,2912,2913,2914,2915,2916,2917,2918,2919,2920,2921,2922,2924,2925,2926,2927,2928,2929,2930,2931,2932,2933,2934,2935,2936,2937,2938,2939,2941,2942,2943,2944,2945,2946,2947,2948,2949,2950,2951,2952,2953,2954,2955,2956,2957,2958,2959,2960,2961,2963,2964,2965,2966,2967,2968,2969,2970,2971,2972,2973,2974,2975,2976,2977,2978,2979,2980,2981,2982,2983,2984,2985,2986,2987,2988,2989,2990,2991,2992,2993,2994,2995,2996,2997,2998,2999,3000,3001,3002,3003,3004,3005,3006,3007,3008,3009,3010,3011,3012,3013,3014,3015,3016,3017,3018,3019,3020,3021,3022,3023,3024,3025,3026,3027,3028,3029,3030,3031,3032,3033,3034,3035,3036,3037,3038,3039,3040,3041,3042,3043,3044,3045,3046,3047,3048,3049,3050,3051,3052,3053,3054,3055,3056,3057,3058,3059,3060,3061,3062,3063,3063,3064,3065,3066,3067,3068,3069,3070,3071,3072,3073,3074,3075,3076,3077,3078,3079,3080,3081,3082,3083,3084,3084,3085,3086,3087,3088,3089,3090,3091,3092,3093,3094,3095,3096,3097,3098,3099,3100,3100,3101,3102,3103,3104,3105,3106,3107,3108,3109,3110,3111,3112,3113,3114,3114,3115,3116,3117,3118,3119,3120,3121,3122,3123,3124,3125,3126,3126,3127,3128,3129,3130,3131,3132,3133,3134,3135,3136,3137,3137,3138,3139,3140,3141,3142,3143,3144,3145,3146,3146,3147,3148,3149,3150,3151,3152,3153,3154,3155,3156,3156,3157,3158,3159,3160,3161,3162,3163,3164,3164,3165,3166,3167,3168,3169,3170,3171,3172,3172,3173,3174,3175,3176,3177,3178,3179,3180,3180,3181,3182,3183,3184,3185,3186,3187,3187,3188,3189,3190,3191,3192,3193,3194,3194,3195,3196,3197,3198,3199,3200,3201,3201,3202,3203,3204,3205,3206,3207,3207,3208,3209,3210,3211,3212,3213,3213,3214,3215,3216,3217,3218,3219,3219,3220,3221,3222,3223,3224,3225,3225,3226,3227,3228,3229,3230,3231,3231,3232,3233,3234,3235,3236,3237,3237,3238,3239,3240,3241,3242,3242,3243,3244,3245,3246,3247,3247,3248,3249,3250,3251,3252,3252,3253,3254,3255,3256,3257,3257,3258,3259,3260,3261,3262,3262,3263,3264,3265,3266,3267,3267,3268,3269,3270,3271,3272,3272,3273,3274,3275,3276,3276,3277,3278,3279,3280,3281,3281,3282,3283,3284,3285,3285,3286,3287,3288,3289,3289,3290,3291,3292,3293,3294,3294,3295,3296,3297,3298,3298,3299,3300,3301,3302,3302,3303,3304,3305,3306,3306,3307,3308,3309,3310,3310,3311,3312,3313,3314,3314,3315,3316,3317,3318,3318,3319,3320,3321,3322,3322,3323,3324,3325,3326,3326,3327,3328,3329,3329,3330,3331,3332,3333,3333,3334,3335,3336,3337,3337,3338,3339,3340,3340,3341,3342,3343,3344,3344,3345,3346,3347,3347,3348,3349,3350,3351,3351,3352,3353,3354,3354,3355,3356,3357,3358,3358,3359,3360,3361,3361,3362,3363,3364,3364,3365,3366,3367,3367,3368,3369,3370,3371,3371,3372,3373,3374,3374,3375,3376,3377,3377,3378,3379,3380,3380,3381,3382,3383,3383,3384,3385,3386,3386,3387,3388,3389,3389,3390,3391,3392,3392,3393,3394,3395,3395,3396,3397,3398,3398,3399,3400,3401,3401,3402,3403,3404,3404,3405,3406,3407,3407,3408,3409,3410,3410,3411,3412,3413,3413,3414,3415,3416,3416,3417,3418,3419,3419,3420,3421,3421,3422,3423,3424,3424,3425,3426,3427,3427,3428,3429,3429,3430,3431,3432,3432,3433,3434,3435,3435,3436,3437,3438,3438,3439,3440,3440,3441,3442,3443,3443,3444,3445,3445,3446,3447,3448,3448,3449,3450,3450,3451,3452,3453,3453,3454,3455,3456,3456,3457,3458,3458,3459,3460,3461,3461,3462,3463,3463,3464,3465,3465,3466,3467,3468,3468,3469,3470,3470,3471,3472,3473,3473,3474,3475,3475,3476,3477,3478,3478,3479,3480,3480,3481,3482,3482,3483,3484,3485,3485,3486,3487,3487,3488,3489,3489,3490,3491,3492,3492,3493,3494,3494,3495,3496,3496,3497,3498,3498,3499,3500,3501,3501,3502,3503,3503,3504,3505,3505,3506,3507,3507,3508,3509,3509,3510,3511,3512,3512,3513,3514,3514,3515,3516,3516,3517,3518,3518,3519,3520,3520,3521,3522,3522,3523,3524,3525,3525,3526,3527,3527,3528,3529,3529,3530,3531,3531,3532,3533,3533,3534,3535,3535,3536,3537,3537,3538,3539,3539,3540,3541,3541,3542,3543,3543,3544,3545,3545,3546,3547,3547,3548,3549,3549,3550,3551,3551,3552,3553,3553,3554,3555,3555,3556,3557,3557,3558,3559,3559,3560,3561,3561,3562,3563,3563,3564,3565,3565,3566,3567,3567,3568,3569,3569,3570,3571,3571,3572,3573,3573,3574,3574,3575,3576,3576,3577,3578,3578,3579,3580,3580,3581,3582,3582,3583,3584,3584,3585,3586,3586,3587,3587,3588,3589,3589,3590,3591,3591,3592,3593,3593,3594,3595,3595,3596,3597,3597,3598,3598,3599,3600,3600,3601,3602,3602,3603,3604,3604,3605,3605,3606,3607,3607,3608,3609,3609,3610,3611,3611,3612,3612,3613,3614,3614,3615,3616,3616,3617,3618,3618,3619};






};


#endif
