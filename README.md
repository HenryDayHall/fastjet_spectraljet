# Fastjet plugin for Spectraljet

This is a shallow interface for [spectraljet](https://github.com/HenryDayHall/spectraljet),
which is itself a python package.
Spectraljet is not a fast algorithm to start with, and python is not a fast language,
so there is some irony in this being a fastjet plugin.
None the less, the algorithm has some interesting behaviour, that we wish to test on
more general data.
The most convenient way to do this is a fastjet plugin.

## Installation

### Installing spectraljet
In order to use this package, you will need 
[spectraljet](https://github.com/HenryDayHall/spectraljet),
So start with the installation instructions there.
Once that is working, check it is available as a module
from any other location.
If you should get;


```bash
$ python3 -m spectraljet.interface
Error, no arguments found
```

If instead you get some variation of a `ModuleNotFoundError`,
try adding the directory you installed
spectraljet to your `$PYTHONPATH`;

```bash
export PYTHONPATH="${PYTHONPATH}:/path/to/folder/spectraljet/"
```

There are probably more elegant ways to solve that,
but this works.

### Installing fastjet

You likely already have fastjet installed,
but if you don't, there are instructions at
[http://fastjet.fr/quickstart.html](http://fastjet.fr/quickstart.html).

### Installing the plugin

Now, from inside this repository, doing;

```bash
$ make
```
should be sufficient.

To check everything worked try;

```bash
$ make example
$ example < example_data.dat
# read an event with 354 particles                                                                   
Set up Spectral plugin                                                                               
#--------------------------------------------------------------------------                          
#                         FastJet release 3.3.4                                                      
#                 M. Cacciari, G.P. Salam and G. Soyez                                               
#     A software package for jet finding and analysis at colliders                                   
#                           http://fastjet.fr                                                        
#                                                                                                    
# Please cite EPJC72(2012)1896 [arXiv:1111.6097] if you use this package                             
# for scientific work and optionally PLB641(2006)57 [hep-ph/0512210].                                
#                                                                                                    
# FastJet is provided without warranty under the GNU GPL v2 or higher.                               
# It uses T. Chan's closest pair algorithm, S. Fortune's Voronoi code                                
# and 3rd party plugin jet algorithms. See COPYING file for details.                                 
#--------------------------------------------------------------------------                          
Getting kinematics                                                                                   
Trying a python call                                                                                 
                                                                                                     
Ran SpectralLong                                                                                     
Printing inclusive jets with pt > 5 GeV                                                              
---------------------------------------                                                              
jet #   rapidity        phi         pt          m          e n constituents                          
    0     -0.867      2.905    983.280     36.457   1377.658       34                                
    1      0.221      6.030    898.497     45.838    921.699       28                                
    2     -2.818      6.092     67.431    359.570   3072.139       38                                
    3      0.331      0.570     16.511      8.539     19.615       23                                
    4     -1.239      0.769      8.323      5.915     19.099       14                                
    5     -2.469      1.034      7.988      3.233     51.230        9                                
    6     -1.637      4.019      7.600      3.989     22.902        7                                
    7     -4.774      4.081      5.720      6.655    519.282       16                                
    8     -4.358      1.467      5.694      4.408    281.330       12 

```
Hopefully that's what you got?

If not, feel free to get in touch/send me a message.
 
