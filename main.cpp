#include <iostream>
#include <cstring>
using namespace std;


/*The contents of this class is meant to be anything fairly
universal to older, more basic cyphers. To cut down on resources
and make the constructors of those methods more specific to the
method at hand.*/
class Cypher {
private:
    const char AlphaL[26] = {'a','b','c','d','e','f','g','h',
                            'i','j','k','l','m','n','o','p',
                            'q','r','s','t','u','v','w','x',
                            'y','z'};
    const char AlphaU[26] = {'A','B','C','D','E','F','G','H',
                             'I','J','K','L','M','N','O','P',
                             'Q','R','S','T','U','V','W','X',
                             'Y','Z'};
    const char* pAlphL = AlphaL;
    const char* pAlphU = AlphaU;
public:
    Cypher() {}

    ~Cypher() {
        if(*pAlphL != NULL && *pAlphU != NULL) {
            delete pAlphL;
            delete pAlphU;
        }
    }

    Cypher(const Cypher& CopySource) {
        if((this != &CopySource) && (CopySource.pAlphU != NULL) &&
           (CopySource.pAlphL != NULL)) {
            if(pAlphL != NULL)
                delete pAlphL;
            if(pAlphU != NULL)
                delete pAlphU;

            pAlphL = new char[26];
            pAlphU = new char[26];

            pAlphL = AlphaL;
            pAlphU = AlphaU;
           }
    }

    Cypher(Cypher&& MoveSource) {
        if((MoveSource.pAlphL != NULL) && (MoveSource.pAlphU != NULL)) {
            pAlphL = MoveSource.pAlphL;
            MoveSource.pAlphL = NULL;
            pAlphU = MoveSource.pAlphU;
            MoveSource.pAlphU = NULL;
        }
    }

    inline const char getLCase(int index) const {
        if(index < 26)
            return pAlphL[index];
    }

    inline const char getUCase(int index) const {
        if(index < 26)
            return pAlphU[index];
    }

    void getInput(string& UsrString) {
        cout << "Enter a string to encrypt: ";
        getline(cin,UsrString);
    }

    void getInput(string& UsrString, string& ciphWord) {
        cout << "Please enter the string to encrypt: ";
        getline(cin,UsrString);

        cout << "Please enter a cypher word (max 45 characters): ";
        cin >> ciphWord;
    }

    const void displayEncryptedString(const string& EnString) {
        cout << EnString << endl;
    }

    const void displayEncryptBuffer(const char* EnWBuffer) {
        cout << EnWBuffer << endl;
    }
};

/*As the title suggests, this is a Caesar Cypher. Which
really is just a 3 letter shift, cypher. The method utilizes
a little discrete math, because it relies on letter position
in the alphabet. Type casting is only used because it's easier
to deal with punctuation/non-alpha characters that way.*/
class CaesarCypher: private Cypher {
private:
    string UsrString;
    unsigned int counter = 0;
    char StringEllie;

public:

    CaesarCypher() {
        cout << "Rolling the die..." << endl;
        getInput(UsrString);
        EncryptString();
        displayEncryptedString(UsrString);
    }

    inline char operator== (const char& CompareTo) {
       return StringEllie == CompareTo;
    }

    void EncryptString() {
        while(counter < UsrString.length()) {
            for(int i = 0; i < 26; ++i) {
                if(UsrString[counter] == getLCase(i)) {
                    if((i-3) < 0) {
                        i -= 3;
                        i += 26;
                        UsrString[counter] = getLCase(i);
                        ++counter;
                    } else {
                        i -= 3;
                        UsrString[counter] = getLCase(i);
                        ++counter;
                    }
                } else if(UsrString[counter] == getUCase(i)) {
                    if((i-3) < 0) {
                        i -= 3;
                        i += 26;
                        UsrString[counter] = getUCase(i);
                        ++counter;
                    } else {
                        i -= 3;
                        UsrString[counter] = getUCase(i);
                        ++counter;
                    }
                } else if((int) UsrString[counter] < 66 || ((int) UsrString[counter] < 97 && (int) UsrString[counter] > 123)) {
                    ++counter;
                }
            }
        }
    }
};

class PolyAlphCipher: private Cypher {
private:
    string ciphWord = "";
    string usrLine = "";
    char* ciphWBuffer;
    char* usrWBuffer;
    int* cypherNDexes = 0;
    int cNdexes[45] = {0};

public:

    PolyAlphCipher() {
        getInput(usrLine,ciphWord);
        SetCiAndUWord(usrLine,ciphWord);
        CypherNdexes();
        EncryptString();
        displayEncryptBuffer(usrWBuffer);
    }

    ~PolyAlphCipher() {
        if(ciphWBuffer != NULL)
            delete[] ciphWBuffer;
        if(usrWBuffer != NULL)
            delete[] usrWBuffer;
        if(cypherNDexes != NULL)
            delete[] cypherNDexes;
    }

    void SetCiAndUWord(string uWord, string cWord) {
        ciphWBuffer = new char[cWord.length() + 1];
        strcpy(ciphWBuffer,cWord.c_str());

        usrWBuffer = new char[uWord.length() + 1];
        strcpy(usrWBuffer,uWord.c_str());
    }

    void CypherNdexes() {
        for(int i = 0; i < ciphWord.length(); ++i) {
            for(int r = 0; r < 26; ++r) {
                if(ciphWBuffer[i] == getLCase(r)) {
                    cNdexes[i] = r;
                } else if(ciphWBuffer[i] == getUCase(r)) {
                    cNdexes[i] = r;
                }
            }
        }
        cypherNDexes = new int[ciphWord.length()];
        cypherNDexes = cNdexes;
    }

    void EncryptString() {
        int x = 0;
        int stringPos = 0;
        int counter = 0;
        while(counter < usrLine.length()) {
            if(stringPos == (ciphWord.length() + 1))
                stringPos = 0;

            for(int i = 0; i < usrLine.length(); ++i) {
                for(int r = 0; r < 26; ++r) {
                    if(usrWBuffer[i] == getLCase(r)) {
                        if((cypherNDexes[stringPos] - r) < 0) {
                            x = (cypherNDexes[stringPos] - r);
                            if(x < 0) {
                                x += 26;
                            } else if (x > 26) {
                                x -= 26;
                            }
                            usrWBuffer[i] = getLCase(x);
                            ++stringPos;
                            ++counter;
                        } else {
                            x = (cypherNDexes[stringPos] - r);
                            usrWBuffer[i] = getLCase(x);
                            ++stringPos;
                            ++counter;
                        }
                    } else if(usrWBuffer[i] == getUCase(r)) {
                        if((cypherNDexes[stringPos] - r) < 0) {
                            x = (cypherNDexes[stringPos] - r);
                            if(x < 0) {
                                x += 26;
                            } else if (x > 26) {
                                x -= 26;
                            }
                            usrWBuffer[i] = getUCase(x);
                            ++stringPos;
                            ++counter;
                        } else {
                            x = (r - cypherNDexes[stringPos]);
                            usrWBuffer[i] = getUCase(x);
                            ++stringPos;
                            ++counter;
                        }
                    } else if((int) usrWBuffer[i] < 66 || ((int) usrWBuffer[i] < 97 && (int) usrWBuffer[i] > 123)) {
                    ++counter;
                    }
                }
            }
        }
    }
};

int main()
{
    PolyAlphCipher PAC;

    cout << "Press q to quit: ";
    char quit;
    cin >> quit;

    return 0;
}
