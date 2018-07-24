// Main fuer Teilaufgabe 1
// Bitte weitere Testfaelle hinzufuegen.
#include "RE.h"
#include <iostream>
#include <stdbool.h>

using namespace std;

RE *deriv(RE *r, char l) {
    switch (r->ofType()) {
        case PhiType:
            return r;
        case EpsType:
            return new Phi();
        case ChType:
            if (((Ch *) r)->getChar() == l) {
                return new Eps();
            } else {
                return new Phi();
            }
        case StarType: {
            RE *r1 = ((Star *) r)->getRE();
            return new Conc(deriv(r1, l), r);
        }
        case AltType: {
            RE *r1 = ((Alt *) r)->getLeft();
            RE *r2 = ((Alt *) r)->getRight();
            return new Alt(deriv(r1, l), deriv(r2, l));
        }
        case ConcType: {
            RE *r1 = ((Conc *) r)->getLeft();
            RE *r2 = ((Conc *) r)->getRight();
            if (r1->containsEps()) {
                return new Alt(new Conc(deriv(r1, l), r2), deriv(r2, l));
            } else {
                return new Conc(deriv(r1, l), r2);
            }
        }
    }// switch
}

bool match(RE *r, string s) {
    for (int i = 0; i < s.length(); i++) {
        r = deriv(r, s[i]);
    }
    return r->containsEps();
}

bool orakel(RE *r, RE *rSimp, string s) {
    return (match(r, s) == match(rSimp, s));
}

typedef enum {
    FAIL,
    OK
} Test;

typedef struct {
    RE *rInput;
    RE *rSimpInput;
    string sInput;
} TestCase;

Test testRE(RE *r, RE *rSimp, string s) {
    if (orakel(r, rSimp, s) == 1) {
        return OK;
    } else {
        return FAIL;
    }
}

void runTests(int no, TestCase test[]) {
    Test t;
    int i;
    string testOutput;

    for (int i = 0; i < no; i++) {
        t = testRE(test[i].rInput, test[i].rSimpInput, test[i].sInput);
        if (t == OK) testOutput = "OK";
        if (t == FAIL) testOutput = "FAIL";
        cout << "Test" << i << ": " << testOutput << "\t r: " << test[i].rInput->pretty() << "\t rSimp: "
             << test[i].rSimpInput->pretty() << "\t s: " << test[i].sInput << endl;
    }
}


int main() {


    cout << "" << endl;
    cout << "Testrahmen für RE: " << endl;

    const int testNo = 8;
    TestCase tests[testNo] = {
            {new Alt(new Phi(), new Ch('c')),                                           new Ch('c'), "c"},
            {new Alt(new Phi(), new Ch('c')),                                           new Ch('c'), "d"},
            {new Conc(new Phi(), new Ch('c')),                                          new Phi(),   "c"},
            {new Star(new Phi()),                                                       new Eps(),   "c"},
            {new Alt(new Alt(new Phi(), new Ch('c')), new Star(new Star(new Ch('c')))), new Alt(new Ch('c'), new Star(
                    new Ch('c'))),                                                                   "d"},
            {new Alt(new Alt(new Phi(), new Ch('c')), new Star(new Star(new Ch('c')))), new Alt(new Ch('c'), new Star(
                    new Ch('c'))),                                                                   "c"},
            {new Alt(new Ch('c'), new Ch('c')),                                         new Ch('c'), "c"},
            {new Alt(new Ch('c'), new Ch('c')),                                         new Ch('c'), "d"}
    };
    runTests(testNo, tests);

    cout << endl << "Test Regel 1:" << endl;

    RE *r10 = new Conc(new Eps(), new Ch('a')); //eps a -> a
    RE *r11 = new Conc(new Eps(), new Conc(new Ch('a'), new Ch('b')));


    const int testNo1 = 5;
    TestCase tests1[testNo1] = {
            {r10, r10->simp(), "d"},
            {r10, r10->simp(), "a"},
            {r11, r11->simp(), "ab"},
            {r11, r11->simp(), "de"},
            {r11, r11->simp(), "a"}
    };
    runTests(testNo1, tests1);


    cout << endl << "Test Regel 2:" << endl;

    RE *r20 = new Conc(new Phi(), new Ch('a'));
    RE *r21 = new Conc(new Ch('e'), new Phi());

    const int testNo2 = 4;
    TestCase tests2[testNo2] = {
            {r20, r20->simp(), "a"},
            {r20, r20->simp(), "b"},
            {r21, r21->simp(), "e"},
            {r21, r21->simp(), "d"}
    };
    runTests(testNo2, tests2);


    cout << endl << "Test Regel 3:" << endl;

    RE *r30 = new Star(new Phi());

    const int testNo3 = 2;
    TestCase tests3[testNo3] = {
            {r30, r30->simp(), " "},
            {r30, r30->simp(), "b"},
    };
    runTests(testNo3, tests3);


    cout << endl << "Test Regel 4:" << endl;

    RE *r40 = new Star(new Star(new Ch('a')));
    RE *r41 = new Star(new Star(new Phi()));
    RE *r42 = new Star(new Star(new Conc(new Ch('a'), new Ch('b'))));

    const int testNo4 = 6;
    TestCase tests4[testNo4] = {
            {r40, r40->simp(), " "},
            {r40, r40->simp(), "a"},
            {r41, r41->simp(), " "},
            {r41, r41->simp(), "a"},
            {r42, r42->simp(), " "},
            {r42, r42->simp(), "ab"},
    };
    runTests(testNo4, tests4);

    cout << endl << "Test Regel 5:" << endl;

    RE *r50 = new Alt(new Ch('c'), new Ch('c'));
    RE *r51 = new Alt(new Conc(new Ch('c'), new Ch('d')),new Conc(new Ch('c'), new Ch('d')));


    const int testNo5 = 6;
    TestCase tests5[testNo5] = {
            {r50, r50->simp(), "c"},
            {r50, r50->simp(), " "},
            {r50, r50->simp(), "d"},
            {r51, r51->simp(), " "},
            {r51, r51->simp(), "cd"},
            {r51, r51->simp(), "a"},
    };
    runTests(testNo5, tests5);


    cout << endl << "Test Regel 6:" << endl;

    RE *r60 = new Alt(new Phi(), new Ch('c'));
    RE *r61 = new Alt(new Phi(),new Conc(new Ch('c'), new Ch('d')));


    const int testNo6 = 6;
    TestCase tests6[testNo6] = {
            {r60, r60->simp(), "c"},
            {r60, r60->simp(), " "},
            {r60, r60->simp(), "d"},
            {r61, r61->simp(), " "},
            {r61, r61->simp(), "cd"},
            {r61, r61->simp(), "a"},
    };
    runTests(testNo6, tests6);

    cout << endl << "Test Regel 7:" << endl;

    RE *r70 = new Alt( new Ch('c'), new Phi());
    RE *r71 = new Alt(new Conc(new Ch('c'), new Ch('d')), new Phi());


    const int testNo7 = 6;
    TestCase tests7[testNo7] = {
            {r70, r70->simp(), "c"},
            {r70, r70->simp(), " "},
            {r70, r70->simp(), "d"},
            {r71, r71->simp(), " "},
            {r71, r71->simp(), "cd"},
            {r71, r71->simp(), "a"},
    };
    runTests(testNo7, tests7);

    cout << endl << "Test if Test can Fail:" <<endl;

    const int testNo8 = 1;
    TestCase tests8[testNo8] = {
            {new Alt(new Phi(), new Ch('a')), new Ch('b'), "a"}
    };
    runTests(testNo8, tests8);
}