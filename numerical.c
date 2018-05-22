#include <math.h>
#include <float.h>

#include "numerical.h"
#include "common.h"
#include "test.h"

u8 dne(double d1, double d2){
    return fabs(d1 - d2) > 0.0000000001;
}

u8 deq(double d1, double d2){
    return !dne(d1, d2);
}

u8 dlt(double d1, double d2){
    return dne(d1, d2) && d1 < d2;
}

u8 dgt(double d1, double d2){
    return dne(d1, d2) && d1 > d2;
}

double numerical_rk_4th_order(numerical_fxy fxy, double x0, double y0, double xn, double h){
    double yn = y0;
    do{
        double k1 = h*fxy(x0, yn);
        double k2 = h*fxy(x0 + h/2, yn + k1/2);
        double k3 = h*fxy(x0 + h/2, yn + k2/2);
        double k4 = h*fxy(x0 + h, yn + k3);
        yn = yn + (k1 + 2*k2 + 2*k3 + k4)/6;
        x0 += h;
    } while(dlt(x0, xn));
    return yn;
}

double numerical_simpsons_1_3rd(numerical_fx fx, double x0, double xn, double h){
    double Is = fx(x0) + fx(xn);
    i64 counter = 1;
    x0 += h;
    do{
        double v = fx(x0);
        if(counter % 2)
            Is += 4*v;
        else
            Is += 2*v;

        counter++;
        x0 += h;
    } while(dlt(x0, xn));
    
    Is *= (h / 3);
    return Is;
}

double numerical_trapezoidals(numerical_fx fx, double x0, double xn, double h){
    double It = fx(x0) + fx(xn);
    x0 += h;
    do{
        It += (2 * fx(x0));
        x0 += h;
    } while(dlt(x0, xn));

    It *= (h / 2);
    return It;
}

double numerical_bisection(numerical_fx fx, double a, double b, double epsilon, i64 maxit){
    if(fx(a) * fx(b) > 0)
        return NAN;
    i64 it = 0;
    double prev = a, nw = a;
    do{
        prev = nw;
        double mid = (a+b)/2;
        nw = mid;
        if(fx(a) * fx(mid) < 0)
            b = mid;
        else
            a = mid;
        it++;
    } while(it < maxit && (it < 2 || fabs(prev - nw) > epsilon));
    
    if(it <= maxit)
        return nw;
    else
        return NAN;
}

double numerical_newton_raphson(numerical_fx fx, numerical_fx fdx, double initial, double epsilon, i64 maxit){
    double root = initial;
    double prev = initial;
    i64 it = 0;
    do{
        prev = root;
        root = root - (fx(prev) / fdx(prev));
        it++;
    } while(it < maxit && fabs(prev - root) > epsilon);
    if(it <= maxit)
        return root;
    else
        return NAN;
}

double numerical_regula_falsi(numerical_fx fx, double a, double b, double epsilon, i64 maxit){
    if(fx(a) * fx(b) > 0)
        return NAN;
    i64 it = 0;
    double pve = fx(a) > 0 ? a : b;
    double nve = pve == a ? b : a;
    double prev = pve, nw = nve;
    do{
        double v = (nve*fx(pve) - pve*fx(nve))/(fx(pve) - fx(nve));
        if(fx(v) >= 0)
            pve = v;
        else
            nve = v;
        prev = nw;
        nw = v;
        it++;
    } while(it < maxit && fabs(prev - nw) > epsilon);
    if(it <= maxit)
        return nw;
    else
        return NAN;
}

/* x0 = 0, y0 = 1
 * xn = 1, h = 0.1
 * expected yn = 1.0715783953
 */
static double rk4th_test_fxy1(double x, double y){
    return (5*x*x - y)/exp(x+y);
}

/* x0 = 0, y0 = 0.5
 * xn = 2, h = 0.05
 * expected yn = 5.305471950534677
 */
static double rk4th_test_fxy2(double x, double y){
    return y - (x*x) + 1;
}

static u8 test_rk4th(){
    double res = numerical_rk_4th_order(rk4th_test_fxy1, 0, 1, 1, 0.1);
    if(dne(res, 1.0715783953))
        return 0;
    res = numerical_rk_4th_order(rk4th_test_fxy2, 0, 0.5, 2, 0.05);
    if(fabs(res - 5.305471) > 0.000001)
        return 0;
    return 1;
}

/* a = 0, b = 2, h = 0.01
 * expected I = 4
 */
static double intg_test_fx1(double x){
    return 2*x;
}

/* a = -3, b = 3, h = 0.1
 * expected I = 97.2
 */
static double intg_test_fx2(double x){
    return x*x*x*x;
}

/* a = 0, b = 1, h = 0.1
 * expected I = 1.4626773
 */
static double intg_test_fx3(double x){
    return exp(x*x);
}

static u8 test_simpsons(){
    double res = numerical_simpsons_1_3rd(intg_test_fx1, 0, 2, 0.01);
    if(dne(res, 4))
        return 0;
    res = numerical_simpsons_1_3rd(intg_test_fx2, -3, 3, 0.1);
    if(fabs(res - 97.2) > 0.001)
        return 0;
    res = numerical_simpsons_1_3rd(intg_test_fx3, 0, 1, 0.1);
    if(fabs(res - 1.4626) > 0.0001)
        return 0;
    return 1;
}

static u8 test_trapezoidals(){
    double res = numerical_trapezoidals(intg_test_fx1, 0, 2, 0.01);
    if(dne(res, 4))
        return 0;
    res = numerical_trapezoidals(intg_test_fx2, -3, 3, 0.1);
    if(fabs(res - 97.2) > 1)
        return 0;
    res = numerical_trapezoidals(intg_test_fx3, 0, 1, 0.1);
    if(fabs(res - 1.46717) > 0.00001)
        return 0;
    return 1;
}

/* a = 0, b = 2.5
 * expected root = 2
 */
static double root_test_fx1(double x){
    return (x*x - 5*x + 6);
}

static double root_test_fx1d(double x){
    return (2*x - 5);
}

/* a = 3, b = 4, epsilon = 0.001
 * expected root = 3.2968
 */
static double root_test_fx2(double x){
    return exp(-x)*(3.2*sin(x) - 0.5*cos(x));
}

static double root_test_fx2d(double x){
    return (-1 * root_test_fx2(x)) + (exp(-x)*(3.2*cos(x) + 0.5*sin(x)));
}

static u8 test_bisection(){
    double res = numerical_bisection(root_test_fx1, 0, 2.5, 0.00000000001, 100);
    if(dne(res, 2))
        return 0;
    res = numerical_bisection(root_test_fx2, 3, 4, 0.001, 100);
    if(fabs(res - 3.2968) > 0.001)
        return 0;
    return 1;
}

static u8 test_regula_falsi(){
    double res = numerical_regula_falsi(root_test_fx1, 0, 2.5, 0.00000000001, 100);
    if(dne(res, 2))
        return 0;
    res = numerical_regula_falsi(root_test_fx2, 3, 4, 0.001, 100);
    if(fabs(res - 3.2968) > 0.001)
        return 0;
    return 1;
}

static u8 test_newton_raphson(){
    double res = numerical_newton_raphson(root_test_fx1, root_test_fx1d, 0, 0.00000000001, 100);
    if(dne(res, 2))
        return 0;
    res = numerical_newton_raphson(root_test_fx2, root_test_fx2d, 3, 0.001, 100);
    if(fabs(res - 3.2968) > 0.001)
        return 0;
    return 1;
}

void test_numericals(){
    tst_suite_start("Numerical Analysis", 6);
    TEST("Runge-Kutta 4th Order", test_rk4th());
    TEST("Simpson's 1/3rd Rule", test_simpsons());
    TEST("Trapezoidal's Rule", test_trapezoidals());
    TEST("Bisection Method", test_bisection());
    TEST("Regula Falsi Method", test_regula_falsi());
    TEST("Newton Raphson Method", test_newton_raphson());
    tst_suite_end();
}
