https://github.com/Un1Gfn/avx

O(n^3)
https://en.wikipedia.org/wiki/Computational_complexity_of_mathematical_operations#Matrix_algebra

GLib
https://developer.gnome.org/glib/unstable/glib-Arrays.html#g-array-new

Least Squares
https://en.wikipedia.org/wiki/Least_squares
https://en.wikipedia.org/wiki/Linear_regression
Dataset
https://www.displayr.com/what-is-linear-regression/

GSL - GNU Scientific Library
https://www.gnu.org/software/gsl/gsl.html
https://www.gnu.org/software/gsl/doc/html/lls.html#c.gsl_fit_linear
https://stackoverflow.com/questions/10821111/gsl-statistics-what-is-stride

PLplot scatter chart
https://en.wikipedia.org/wiki/PLplot
http://plplot.org/
http://plplot.sourceforge.net/
https://sourceforge.net/projects/plplot/
http://plplot.sourceforge.net/examples.php?demo=00&lbind=C


export port=64535
export ip=3.81.33.151
proxychains ssh -p $port $ip

# https://unix.stackexchange.com/questions/77127/rm-rf-all-files-and-all-hidden-files-without-error
proxychains ssh -p $port $ip bash <<<'cd /root/project/; rm -rf ..?* .[!.]* *'; proxychains scp -rp -P $port /home/darren/avx/*.{c,h,sh} root@$ip:/root/project/

proxychains scp -rp -P $port /home/darren/avx/run.c root@$ip:/root/project/
proxychains scp -rp -P $port /home/darren/avx/main.c root@$ip:/root/project/

gdb --args ./a.out run 0 1000 10

set confirm off
break dgemm_avx.c:16
run

print C
print i
print j
print n

malloc_usable_size()


