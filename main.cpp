#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

///KAMUS STRUCT
typedef struct{
    string symbiotesName;
    int attack;
    int hp;
}SymbiotesList;

typedef struct{
    int level;
    int indexMon;
    int currentHP;
}SymbiotesStat;

typedef struct{
    vector<SymbiotesStat> listSymbiotes;
    int maxCap;
}Symbiotes;

///KAMUS GLOBAL
vector<vector<SymbiotesList>> poolSymbiotes {
    {
        {"Ignis",25,225},
        {"Aqua",22,228},
        {"Terram",18,232}
    },
    {
        {"Fotia",50,450},
        {"Nero",44,456},
        {"GI",36,264}
    },
    {
        {"BOSS",100,1000}
    }
};
string playerName, lokasi, status;

///KAMUS FUNGSI
void pressAny(){
    //press Any key
    system("PAUSE");
    system("CLS");
}

void clearCin(){
    //Clear Input
    cin.clear();
    cin.ignore(256,'\n');
    cout << "\n:( input salah, silahkan input ulang" << endl;
    pressAny();
}

void tampilDaftarSymbiotes(Symbiotes s);
void tampilIntro();
void tampilTamat();
void tampilStatus();
void tutorial(Symbiotes *f, Symbiotes *i, Symbiotes *e);
void pushEnemy(Symbiotes *e, int jumlah, bool acakLevel);
bool bertualangScene(Symbiotes *f, Symbiotes *i, Symbiotes *e);
void attackScene(Symbiotes *p, Symbiotes *e);
void tangkapScene(Symbiotes *enemy, Symbiotes *invenSymbiotes);
void evolusiScene(Symbiotes *i);
int pilihEvolusi(Symbiotes i);
void pindah(Symbiotes *to,Symbiotes *from);
void buang(Symbiotes *i);
void daftarSymbiotesScene(Symbiotes *formasi, Symbiotes *invenSymbiotes);
void attackTurn(Symbiotes *attacker, Symbiotes *defense, bool playerTurn);
void tampilStatusAttack(Symbiotes player, Symbiotes enemy);
bool kalah(Symbiotes s);
void sembuh50(Symbiotes *f);
void sembuhFull(Symbiotes *s);


int main()
{
    ///KAMUS
    Symbiotes formasiSymbiotes,invenSymbiotes, enemySymbiotes, boss;
    formasiSymbiotes.maxCap = 3;
    invenSymbiotes.maxCap = 7;
    enemySymbiotes.maxCap = formasiSymbiotes.maxCap;
    lokasi = "Tutorial";
    status = "Intro";
    int tindakan;
    bool tamat = false, keluar = false, bertualangSelesai = false;

    ///START & LOADING
    system("CLS");
    cout << "Start" << endl;
    pressAny();
    for(int load = 0; load <= 100;load+=5){
        system("CLS");
        cout << "Loading..." << load << "%" <<endl;
    }
    system("CLS");

    ///INTRO
    tampilIntro();

    ///TUTORIAL
    tutorial(&formasiSymbiotes, &invenSymbiotes, &enemySymbiotes);

    ///MAIN GAME PROGRAM
    while(!tamat && !keluar){
        lokasi = "Laboratorium";
        status = "IDLE";
        sembuhFull(&formasiSymbiotes);
        sembuhFull(&invenSymbiotes);
        tampilStatus();
        cout << "Formasi" << endl;
        tampilDaftarSymbiotes(formasiSymbiotes);
        cout << "1. Inventory" << endl;
        cout << "2. Bertualang" << endl;
        cout << "3. Lawan Boss" << endl;
        cout << "4. Keluar" << endl;
        cout << "input : " << endl;
        cin >> tindakan;
        switch(tindakan){
            case 1:{
                daftarSymbiotesScene(&formasiSymbiotes, &invenSymbiotes);
                break;
            }
            case 2:{
                bertualangSelesai = bertualangScene(&formasiSymbiotes, &invenSymbiotes, &enemySymbiotes);
                if(bertualangSelesai){
                    system("CLS");
                    cout << "Helena\t:" << endl;
                    cout << "\"Bertualang sudah selesai !!!" << endl;
                    cout << " Sekarang kamu dapat melawan BAD SYMBIOTES\"" << endl;
                }
                pressAny();
                break;
            }
            case 3:{
                if(bertualangSelesai){
                    boss.maxCap = 1;
                    if(boss.listSymbiotes.size() < boss.maxCap)
                        boss.listSymbiotes.push_back({2,0,1000});
                    attackScene(&formasiSymbiotes, &boss);
                    if(kalah(boss)){
                        tamat = true;
                    }
                    else{
                        cout << "Jangan patah semangat anda dapat mencoba lain kali lagi" << endl;
                        sembuhFull(&boss);
                    }
                }
                else{
                    cout << "Selesaikan Bertualang dahulu" << endl;
                }
                pressAny();
                break;
            }
            case 4:{
                keluar = true;
                break;
            }
            default:{
                clearCin();
            }
        }
        ///TAMAT
        tampilTamat();
    }
    return 0;
}

void tampilStatus(){
    system("CLS");
    cout << "Status : " << lokasi << " - " << status << endl;
    cout << "Player : " << playerName << endl << endl;
}

void tampilDaftarSymbiotes(Symbiotes s){
    cout << "Daftar Symbiotes\t\t" << endl;
    auto sym = s.listSymbiotes.begin();
    for(int i = 0; i < s.maxCap; ++i){
        cout << "[" << i+1 << "]" << endl;
        if(sym != s.listSymbiotes.end()){
            cout << "Name\t: " << poolSymbiotes[(*sym).level][(*sym).indexMon].symbiotesName << endl;
            cout << "Level\t: " << (*sym).level + 1 << endl;
            cout << "Attack\t: " << poolSymbiotes[(*sym).level][(*sym).indexMon].attack << endl;
            cout << "HP\t: " << (*sym).currentHP << " / " << poolSymbiotes[(*sym).level][(*sym).indexMon].hp << endl << endl;
            ++sym;
        }
        else{
            cout << "Empty" << endl << endl;
        }
    }
}

void tampilStatusAttack(Symbiotes player, Symbiotes enemy){
    tampilStatus();
    cout << playerName << endl;
    tampilDaftarSymbiotes(player);
    cout << "\t\t====== Versus =====" << endl << endl;
    cout << "Musuh" << endl;
    tampilDaftarSymbiotes(enemy);
}

void pushEnemy(Symbiotes *e, int jumlah, bool acakLevel){
    //kamus
    int index,level;
    srand((unsigned)time(0));

    if(e->listSymbiotes.size() > 0)
        e->listSymbiotes.clear();
    for(int i = 0; i < jumlah; ++i){
        index = rand() % 3;
        if(acakLevel)
            level = rand() % 2;
        else
            level = 0;
        e->listSymbiotes.push_back({level, index, poolSymbiotes[level][index].hp});
    }
}

bool bertualangScene(Symbiotes *f, Symbiotes *i, Symbiotes *e){
    //kamus
    int tindakan;
    lokasi = "Bertualang";

    for(int j = 1; j <= 5; ++j){
        if(kalah(*f))
            return false;
        status = "Level " + to_string(j);
        pushEnemy(e, (rand() % e->maxCap)+1, j>3);

        tampilStatus();
        tampilStatusAttack(*f, *e);
        cout << "1. Lawan" << endl;
        cout << "2. Kembali ke Laboratorium" << endl;
        cout << "input : " << endl;
        cin >> tindakan;
        switch(tindakan){
            case 1:{
                attackScene(f, e);
                if(kalah(*e)){
                    tangkapScene(e, i);
                    sembuh50(f);
                }
                else
                    return false;
                break;
            }
            case 2:{
                return false;
                break;
            }
            default:
                clearCin();
        }
    }
    return true;
}

void attackScene(Symbiotes *p, Symbiotes *e){
    bool kabur = false;
    int tindakan;

    while(!kalah(*p) && !kalah(*e) && !kabur){
        tampilStatusAttack(*p, *e);
        cout << "1. Serang" << endl;
        cout << "2. Kabur" << endl;
        cout << "input :" << endl;
        cin >> tindakan;
        switch(tindakan){
            case 1 :{
                //player turn attack
                attackTurn(p, e, true);
                //enemy turn attack
                attackTurn(e, p, false);
                break;
            }
            case 2 :{
                kabur = true;
                break;
            }
            default:
                clearCin();
        }
    }
}

void tangkapScene(Symbiotes *enemy, Symbiotes *invenSymbiotes){
    //kamus
    int tindakan;
    bool skip = false;


    while(!skip && enemy->listSymbiotes.size() > 0){
        tampilStatus();
        cout << "Symbiotes yang dapat ditangkap" << endl;
        tampilDaftarSymbiotes(*enemy);
        cout << "Inventory" << endl;
        tampilDaftarSymbiotes(*invenSymbiotes);
        cout << "1. Tangkap" << endl;
        cout << "2. Lewati" << endl;
        cout << "input :" << endl;
        cin >> tindakan;
        switch(tindakan){
            case 1 :{
                pindah(invenSymbiotes,enemy);
                break;
            }
            case 2 :{
                skip = true;
                break;
            }
            default:{
                clearCin();
            }
        }
    }
}

void daftarSymbiotesScene(Symbiotes *formasi, Symbiotes *invenSymbiotes){
    //KAMUS
    int tindakan,tindakanUbahFormasi,indexMonster;
    bool exit = false;

    while(!exit){
        status = "Inventory";
        tampilStatus();
        cout << "Formasi" << endl;
        tampilDaftarSymbiotes(*formasi);
        cout << "Inventory" << endl;
        tampilDaftarSymbiotes(*invenSymbiotes);
        cout << "1. Ubah Formasi" << endl;
        cout << "2. Buang Symbiotes" << endl;
        cout << "3. Evolusi Symbiotes" << endl;
        cout << "4. Keluar" << endl;
        cout << "input :" << endl;
        cin >> tindakan;
        switch(tindakan){
            case 1:{
                status = "Ubah Formasi";
                tampilStatus();
                cout << "Formasi" << endl;
                tampilDaftarSymbiotes(*formasi);
                cout << "1. Tambahkan" << endl;
                cout << "2. Hapus" << endl;
                cout << "3. Exit" << endl;
                cout << "input :" << endl;
                cin >> tindakanUbahFormasi;
                switch(tindakanUbahFormasi){
                    case 1:{
                        pindah(formasi,invenSymbiotes);
                        break;
                    }
                    case 2:{
                        pindah(invenSymbiotes,formasi);
                        break;
                    }
                    case 3:{
                        break;
                    }
                    default:{
                        clearCin();
                    }
                }
                break;
            }
            case 2:{
                buang(invenSymbiotes);
                break;
            }
            case 3:{
                evolusiScene(invenSymbiotes);
                break;
            }
            case 4:{
                exit = true;
                break;
            }
            default:{
                clearCin();
            }
        }
    }
}

void attackTurn(Symbiotes *attacker, Symbiotes *defense, bool playerTurn){
    int menyerangIndex,index;
    srand((unsigned)time(0));
    index = 1;
    if(playerTurn){
        for(auto i = attacker->listSymbiotes.begin(); i != attacker->listSymbiotes.end() && !kalah(*defense); ++i){
            //monster yang hp nya 0 skip untuk attack turn
            if((*i).currentHP == 0){
                index++;
                continue;
            }
            tampilStatusAttack(*attacker, *defense);

            //nyari index
            cout << "GILIRAN ANDA" << endl;
            cout << "[" << index << "] " << poolSymbiotes[(*i).level][(*i).indexMon].symbiotesName << " : ";
            cin >> menyerangIndex;
            menyerangIndex --;

            //cek inputan
            if(menyerangIndex > defense->listSymbiotes.size()-1 || menyerangIndex < 0 || cin.fail() || defense->listSymbiotes[menyerangIndex].currentHP == 0 ){
                --i;
                clearCin();
                continue;
            }

            defense->listSymbiotes[menyerangIndex].currentHP -= poolSymbiotes[(*i).level][(*i).indexMon].attack;

            //apabila mati
            if(defense->listSymbiotes[menyerangIndex].currentHP <= 0)
                defense->listSymbiotes[menyerangIndex].currentHP = 0;
            index++;
        }
        tampilStatusAttack(*attacker, *defense);
    }
    else{
        cout << "GILIRAN MUSUH" << endl;
        for(auto i = attacker->listSymbiotes.begin(); i != attacker->listSymbiotes.end() && !kalah(*defense); ++i){
            //monster yang hp nya 0 skip untuk attack turn
            if((*i).currentHP == 0){
                index++;
                continue;
            }
            //nyari index
            menyerangIndex = rand() % defense->listSymbiotes.size();

            //apabila menyerang musuh yang HPnya 0
            if(defense->listSymbiotes[menyerangIndex].currentHP == 0){
                for(int i = 0;i < defense->listSymbiotes.size(); i++){
                    if(defense->listSymbiotes[i].currentHP != 0){
                        menyerangIndex = i;
                        break;
                    }
                }
            }
            defense->listSymbiotes[menyerangIndex].currentHP -= poolSymbiotes[(*i).level][(*i).indexMon].attack;
            cout << " ! " << poolSymbiotes[(*i).level][(*i).indexMon].symbiotesName << "[" << index << "]";
            cout << " menyerang anda, mengurangi " << poolSymbiotes[(*i).level][(*i).indexMon].attack << " HP ";
            cout << poolSymbiotes[defense->listSymbiotes[menyerangIndex].level][defense->listSymbiotes[menyerangIndex].indexMon].symbiotesName << "[" << menyerangIndex+1 << "]" << endl;

            //apabila mati
            if(defense->listSymbiotes[menyerangIndex].currentHP < 0)
                defense->listSymbiotes[menyerangIndex].currentHP = 0;
            index++;
        }
        system("PAUSE");
    }
}

bool kalah(Symbiotes s){
    for(auto i = s.listSymbiotes.begin(); i != s.listSymbiotes.end(); ++i){
        if((*i).currentHP != 0)
            return false;
    }
    return true;
}

void pindah(Symbiotes *to,Symbiotes *from){
    //KAMUS
    int indexMonster;


    if(from->listSymbiotes.size() > 0){
        //kalo inven  tidak penuh
        if(to->listSymbiotes.size() < to->maxCap){
            tampilStatus();
            tampilDaftarSymbiotes(*from);
            cout << "input : ";
            cin >> indexMonster;

            //kalo input salah
            if(indexMonster <= from->listSymbiotes.size() && indexMonster > 0){
                indexMonster--;
                to->listSymbiotes.push_back(from->listSymbiotes[indexMonster]);
                cout << "! " << poolSymbiotes[from->listSymbiotes[indexMonster].level][from->listSymbiotes[indexMonster].indexMon].symbiotesName << "[" << indexMonster+1 << "] telah dipilih" << endl;
                from->listSymbiotes.erase(from->listSymbiotes.begin() + indexMonster);
            }
            else{
                clearCin();
            }
        }
        else
            cout << "Penuh ! Hapus terlebih dahulu" << endl;
            pressAny();
        }
    else{
        cout << "Tidak dapat melakukan tindakan" << endl;
        pressAny();
    }
}

void buang(Symbiotes *i){
    //kamus
    int indexMonster;
    status = "Buang Symbiotes";

    if(i->listSymbiotes.size() > 0){
        tampilStatus();
        tampilDaftarSymbiotes(*i);
        cout << "input : ";
        cin >> indexMonster;

        if(indexMonster > 0 && indexMonster <= i->listSymbiotes.size()){
            indexMonster--;
            cout << "! " << poolSymbiotes[i->listSymbiotes[indexMonster].level][i->listSymbiotes[indexMonster].indexMon].symbiotesName << "[" << indexMonster+1 << "] telah dihapus" << endl;
            i->listSymbiotes.erase(i->listSymbiotes.begin()+indexMonster);
            pressAny();
        }
        else{
            clearCin();
        }
    }
    else{
        cout << "Tidak dapat melakukan tindakan" << endl;
        pressAny();
    }
}

void evolusiScene(Symbiotes *i){
    //kamus
    int indexMon1,indexMon2;
    status = "Evolusi Symbiotes";

    if(i->listSymbiotes.size() > 1){ // harus ada minimal 2 symbiotes
        system("CLS");
        cout << "Pilih Symbiotes ke - 1" << endl;
        pressAny();
        indexMon1 = pilihEvolusi(*i);
        cout << "Pilih Symbiotes ke - 2" << endl;
        pressAny();
        indexMon2 = pilihEvolusi(*i);

        if(indexMon1 != indexMon2 && i->listSymbiotes[indexMon1].indexMon == i->listSymbiotes[indexMon2].indexMon){ //index monster harus beda & nama monster yang dievolusikan harus sama
            cout << poolSymbiotes[0][i->listSymbiotes[indexMon1].indexMon].symbiotesName << " berhasil di Evolusikan menjadi " << poolSymbiotes[1][i->listSymbiotes[indexMon1].indexMon].symbiotesName << endl;
            i->listSymbiotes[indexMon1].level++; //menaikan level / evolusi
            i->listSymbiotes.erase(i->listSymbiotes.begin()+indexMon2); // menghapus monster lainnya
        }
        else{
            cout << "Tidak dapat melakukan tindakan" << endl;
        }
    }
    else{
        cout << "Tidak dapat melakukan tindakan, Symbiotes kurang" << endl;
    }
    pressAny();
}

int pilihEvolusi(Symbiotes i){
    //kamus
    int indexMonster;
    bool pilihUlang = true;

    while(pilihUlang){
        tampilStatus();
        tampilDaftarSymbiotes(i);
        cout << "input : ";
        cin >> indexMonster;
        if(indexMonster > 0 && indexMonster <= i.listSymbiotes.size() && i.listSymbiotes[indexMonster-1].level < 1){
            indexMonster--;
            pilihUlang = false;
        }
        else
            clearCin();
    };
    pressAny();
    return indexMonster;
}

void tampilIntro(){
    cout << "\"Jauh di tengah hutan jauh dari perkotaan terdapat sebuah Laboratorium rahasia" << endl;
    cout << " dibawah pemerintahan untuk melakukan sebuah penelitian tentang keberadaan makhluk asing\"" << endl;
    system("PAUSE");
    cout << "\n\"Prof. Andreas dibantu oleh Asistennya Helena dan juga anggotanya yang terpilih " << endl;
    cout << " untuk ikut serta mencoba untuk meneliti sebuah cara agar dapat berkomunikasi dengan makhluk asing " << endl;
    cout << " dan secara tidak sengaja menemukan caranya dan berkomunikasi dengan makhluk asing tersebut, " << endl;
    cout << " walau suara dari makhluk tersebut belum diketahui artinya\"" << endl;
    pressAny();
    cout << "\"Hingga ternyata sinyal tersebut membuat sebuah meteor raksasa terpanggil ke bumi dan jatuh disekitaran Laboratorium, " << endl;
    cout << " setelah dilakukan penelitian terdapat beberapa makhluk hidup yang dibawa oleh meteor itu" << endl;
    cout << " dan telah beradaptasi dengan bumi dan memiliki elemen masing masing yang berbeda  " << endl;
    cout << " dan makhluk ini disebut \"Symbiotes\".\"" << endl;
    system("PAUSE");
    cout << "\n\"Kemampuan dari makhluk ini adalah dapat berkembiang biak dengan sangat cepat " << endl;
    cout << " membuat Prof.Andreas tidak dapat mengatasinya, " << endl;
    cout << " beberapa dari Symbiotes ini kabur dan berkeliaran di laboratorium\"" << endl;
    system("PAUSE");
    cout << "\n\"Terdapat 1 Symbiotes berbahaya berukuran besar yang kabur, " << endl;
    cout << " kekuatannya didapatkan ketika berevolusi dan memakan Symbiotes lainnya." << endl;
    cout << " Prof. Andreas sadar akan potensi bahaya jika Bad Symbiotes itu sampai dikota\"" << endl;
    system("PAUSE");
    cout << "\n\"Maka dari itu anggota dari Prof.Andreas di latih untuk dapat mengendalikan Symbiotes, " << endl;
    cout << " agar dapat mengalahkan BAD SYMBIOTES\"" << endl;
    system("PAUSE");
    cout << "\n\"Karena kamu merupakan anggota dari prof.Andreas, " << endl;
    cout << " maka juga tugasmu untuk mengalah BAD SYMBIOTES\"" << endl;
    pressAny();
}

void tutorial(Symbiotes *f, Symbiotes *i, Symbiotes *e){
    //kamus
    int index,indexPilih;
    bool ulang = true;
    char konfirmasi;

    //input nama
    cout << "Helena\t:"<< endl;
    cout << "\"Maaf ! aku lupa dengan namamu bisa beritahu aku lagi siapa namamu ?\"\n" << endl;
    cout << "input\t:" << endl;
    cin >> playerName;

    ///PILIH MONSTER, Menampilkan Monster dengan Level 1
    while(ulang){
        system("CLS");
        cout << "Helena\t:\n" << "\"Oke " << playerName << " aku akan membantumu untuk dapat mengalahkan BAD SYMBIOTES," << endl;
        cout << " Sebelumnya silahkan pilih Symbiotes pertamamu" << endl;
        cout << " masing masing dari mereka memiliki elemen yang berbeda" << endl;
        cout << " dan tiap elemen saling kuat dan saling lemah terhadap elemen yang lain\"\n" << endl;
        system("PAUSE");

        //tampilkan pool poolSymbiotes level 1
        index = 1;
        for(auto j = poolSymbiotes[0].begin(); j != poolSymbiotes[0].end(); ++j){
            cout << "[" << index << "]" << endl;
            cout << "Name\t: " << (*j).symbiotesName << endl;
            cout << "Level\t: 1" << endl;
            cout << "Attack\t :" << (*j).attack << endl;
            cout << "HP\t: " << (*j).hp << endl << endl;
            index++;
        }

        //input pilih symbiotes
        cout << "Pilih Symbiotes pertamamu (1-3)\t :" << endl;
        cin >> indexPilih;
        if(indexPilih > 0 && indexPilih <= poolSymbiotes[0].size()){
            indexPilih--;

            cout << "\nHelena\t:" << endl;
            cout << "\"Kamu akan memilih \"" << poolSymbiotes[0][indexPilih].symbiotesName << "\" sebagai Symbiotes pertamamu ?\"" << endl;
            cout << "[Y/N]" << endl;
            cin >> konfirmasi;
            if(konfirmasi == 'y' || konfirmasi == 'Y' )
                ulang = false;
            else{
                ulang = true;
                clearCin();
            }
        }
        else
            clearCin();
    };

    ///TAMBAHKAN SYMBIOTES PERTAMA KE INVENTORY
    f->listSymbiotes.push_back({0,indexPilih,poolSymbiotes[0][indexPilih].hp});                 //menambahkan/menyimpan index monster pada daftar Monster, Pool HP di poolSymbiotes dijadikan current HP, 0 karena level pertama sebelum evolusi (level 1 = index 0, level 2 = index 1)
    cout << "\nHelena\t:" << endl;
    cout << "\"Symbiotes \"" << poolSymbiotes[f->listSymbiotes[0].level][f->listSymbiotes[0].indexMon].symbiotesName << "\" telah ditambahkan silahkan lihat daftar Symbiotes milikmu\"" << endl;
    pressAny();

    ///TUTORIAL MENAMPILKAN FORMASI SYMBIOTES
    f->listSymbiotes.push_back({0, 2, 750}); //menambahkan symbiotes kuat untuk melawan symbiotes pertama
    tampilDaftarSymbiotes(*f);
    cout << "\nHelena\t:" << endl;
    cout << "\"Baik, diatas adalah formasi dari Symbiotes milikmu," << endl;
    cout << " untuk membantu mengalahkan Symbiotes kami telah menambahkan 1 Symbiotes" << endl;
    cout << " yang telah dimutasi membuatnya memiliki HP yang tinggi, tapi akan mati setelah pertarungan ini selesai" << endl;
    cout << " Selanjutnya kamu harus melawan Symbiotes yang lain dengan formasi bertarungmu sendiri" << endl;
    cout << " lalu tangkap Symbiotes untuk bertarung bersamamu.\"" << endl;
    pressAny();

    ///TUTORIAL FIRST ATTACK
    status = "Bertarung";
    cout << "Helena\t:" << endl;
    cout << "\"Di depan ada Symbiotes lainnya kalahkan makhluk itu" << endl;
    cout << " lalu tangkap untuk dapat melengkapi formasi bertarungmu.\"" << endl;
    pressAny();
    pushEnemy(e, 3,false);            //generate random monster
    while(true){
        attackScene(f, e);
        if(kalah(*e))
            break;
        system("CLS");
        cout << "Helena\t:" << endl;
        cout << "\"Tutorial anda tidak dapat kabur\"" << endl;
        pressAny();
    }
    f->listSymbiotes.erase(f->listSymbiotes.begin()+1);


    ///TUTORIAL TANGKAP
    status = "Tangkap Symbiotes";
    while(true){
        cout << "Helena\t:" << endl;
        cout << "\"Kamu baru saja berhasil mengalahkan monster," << endl;
        cout << " ketika pertarungan selesai kamu dapat menangkap Symbiotes untuk melengkapi formasi bertarungmu\"" << endl;
        pressAny();
        tangkapScene(e, i);
        if(i->listSymbiotes.size() > 0)
            break;
        system("CLS");
        cout << "Helena\t:" << endl;
        cout << "\"Tutorial anda tidak dapat melewati\"" << endl;
        pressAny();
    }

    ///TUTORIAL UBAH FORMASI
    status = "Ubah Formasi";
    cout << "Helena\t:" << endl;
    cout << "\"Kamu memiliki 2 daftar Symbiotes" << endl;
    cout << " Daftar Formasi adalah Symbiotes yang ikut bertarung," << endl;
    cout << " Daftar Symbiotes disimpan adalah yang tidak ikut bertarung\"" << endl;
    pressAny();
    daftarSymbiotesScene(f, i);

    ///TUTORIAL SELESAI
    status = "IDLE";
    system("CLS");
    cout << "Helena\t:" << endl;
    cout << "\"Tutorial sudah selesai !!!" << endl;
    cout << " Selanjutnya tugasmu adalah perkuat formasi bertarungmu dan kalahkan BAD SYMBIOTES\"" << endl;
    pressAny();
}

void tampilTamat(){
    system("CLS");
    cout << "Helena\t:" << endl;
    cout << "\"Selamat BAD SYMBIOTES sudah dikalahkan !!!" << endl;
    cout << " semua telah berakhir\"" << endl;
    cout << endl;
    system("Pause");
    cout << "Helena\t:" << endl;
    cout << "\"kini tugas Prof.Andreas hanya bagaimana mengembalikan Symbiotes ini" << endl;
    cout << endl;
}


void sembuh50(Symbiotes *f){
    for(auto i = f->listSymbiotes.begin(); i != f->listSymbiotes.end(); ++i){
        (*i).currentHP += 50*((*i).level+1);
        if((*i).currentHP > poolSymbiotes[(*i).level][(*i).indexMon].hp){
            (*i).currentHP = poolSymbiotes[(*i).level][(*i).indexMon].hp;
        }
    }
}
void sembuhFull(Symbiotes *s){
    for(auto i = s->listSymbiotes.begin(); i != s->listSymbiotes.end(); ++i){
        (*i).currentHP = poolSymbiotes[(*i).level][(*i).indexMon].hp;
    }
}
