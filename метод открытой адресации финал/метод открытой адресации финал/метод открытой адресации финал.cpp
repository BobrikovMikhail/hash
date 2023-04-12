#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <time.h>
#include <fstream>
#include <string>
#include <Windows.h>
using namespace std;
struct hashtab {
    int number = -1;
    string FIO = "";
};

struct client {
    char FIO[256] = "";
    int account = 0;
    int summa = 0;
};

// Функция для сохранения массива в файл
void SaveArr(client* arr, int n) {
    client* current = new client;
    char FName[] = "array.txt"; //Путь к файлу
    ofstream f1(FName, ios_base::out | ios_base::binary);   // открываем файл для записи
    for (int i = 0; i < n; i++) {
        //current.FIO = arr[i].FIO;
        strncpy(current->FIO, arr[i].FIO, 256);
        current->account = arr[i].account;
        current->summa = arr[i].summa;
        f1.write((char*)current, sizeof(client));   /*записываем структуру в файл*/
    }
    f1.close();     // закрываем файл
}

// Функция для чтения массива из файла
void LoadArr(client* arr, int n) {
    client current;
    char FName[] = "array.txt"; //Путь к файлу
    ifstream f2(FName);  // открываем файл для чтения
    if (!f2) {
        cout << "Файл не существует\n\n";
        f2.close();
    }
    for (int i = 0; i < n; i++) {
        strncpy(arr[i].FIO, "", 256);
        arr[i].account = 0;
        arr[i].summa = 0;
    }
    f2.seekg(0, ios::end);      //функция перехода в конец файла
    long NumBytes = (long)f2.tellg();  //выяснение кол-ва байтов в файле
    float NumStruct = NumBytes / sizeof(client);  //выяснение кол-ва структур в файле
    f2.seekg(0, ios::beg);      //переход в начало файла
    for (int i = 0; i < NumStruct; i++) {
        f2.read((char*)&current, sizeof(client));  // запись структуры из файла в переменную current
        //cout << current2.FIO << " " << current2.account << " " << current2.summa << endl;
        strncpy(arr[i].FIO, current.FIO, 256);
        arr[i].account = current.account;
        arr[i].summa = current.summa;
        cout << arr[i].FIO << " " << arr[i].account << " " << arr[i].summa << endl;
    }
    f2.close();
}

int get_rnd_number(int max) {
    return rand() % max;
}

string get_rnd_fio() {
    string name[10] = { "Александр", "Борис", "Виктор", "Григорий", "Дмитрий", "Евгений", "Иван", "Николай", "Михаил", "Олег" };
    string patronymic[10] = { "Александрович", "Борисович", "Викторович", "Григорьевич", "Дмитриевич", "Евгеньевич", "Иванович", "Николаевич", "Михайлович", "Олегович" };
    string surname[10] = { "Александров", "Борисов", "Викторов", "Григорьев", "Дмитриев", "Евгеньев", "Иванов", "Николаев", "Михайлов", "Олегов" };
    string fio = name[get_rnd_number(9)] + " " + patronymic[get_rnd_number(9)] + " " + surname[get_rnd_number(9)];
    return fio;
}

void makearr_rnd(int n, client* arr) {
    string s = "";
    char s2[256];
    s = get_rnd_fio();
    strcpy(s2, s.c_str());
    for (int i = 0; i < n - 5; i++) {//-5 - оставляеи 5 пустых строк, чтобы было, куда добавить
        s = get_rnd_fio();
        strcpy(s2, s.c_str());
        strncpy(arr[i].FIO, s2, 256);
        arr[i].account = get_rnd_number(100);
        arr[i].summa = get_rnd_number(1000);
    }
    for (int j = 0; j < n - 5; j++) {
        cout << arr[j].FIO << " " << arr[j].account << " " << arr[j].summa << endl;
    }
    cout << endl;
}

void addEl(client* arr, int n) {
    client input;
    string temp;
    string temp3;
    cout << "Введите ФИО: ";
    cin.ignore();
    getline(cin, temp);
    strcpy(input.FIO, temp.c_str());
    cout << "Введите номер счета: ";
    cin >> input.account;
    cout << "Введите сумму: ";
    cin >> input.summa;
    if (arr == nullptr) {
        cout << "Ошибка";
        return;
    }
    else {
        for (int i = 0; i < n; i++) {
            temp3 = arr[i].FIO;
            if (temp3 == "") {
                arr[i] = input;
                return;
            }
        }
        cout << "В массиве нет места для новой записи" << endl;
    }
}

int hash_function(string str, int n) {
    unsigned long i = 0;
    for (int j = 0; str[j]; j++) {
        i += str[j];
    }
    return i % n;
}

void hashta(client* arr, int n, hashtab* hashtable) {
    int cnt = 0;
    for (int i = 0; i < n; i++) {
        hashtable[i].number = -1;
        hashtable[i].FIO = "";
    }
    for (int i = 0; i < n; i++) {
        if (arr[i].FIO != "") { //строка массива не пуста
            int key = hash_function(arr[i].FIO, n);
            bool found = false;
            if (hashtable[key].number == -1) {
                hashtable[key].number = key;
                hashtable[key].FIO = arr[i].FIO;
                found = true;
            }
            else {
                cnt++;
            }
            int j = (key + 1) % n;
            while (j != key && !found) {
                if (hashtable[j].number == -1) {
                    hashtable[j].number = key;
                    hashtable[j].FIO = arr[i].FIO;
                    found = true;
                }
                j = (j + 1) % n;
            }
        }
    }
    for (int i = 0; i < n; i++) {
        cout << hashtable[i].number << ' ' << hashtable[i].FIO << endl;
    }
    cout << endl;
    cout << "счётчик коллизий: " << cnt << endl;
}

void hashFound(hashtab* hashtable, int n, client* arr) {
    string a;
    int k, tmp, i;
    bool found = false, found2;
    cout << "введите ФИО для поиска: "; cin.ignore(); getline(cin, a);
    k = hash_function(a, n);
    if (hashtable[k].FIO == a) {
        found = true;
        cout << "ответ найден в позиции: " << k << endl;
        found2 = FALSE;
        for (i = 0; i < n && !found2; i++) {
            if (arr[i].FIO == a) {
                found2 = TRUE;
                cout << arr[i].FIO << ' ' << arr[i].account << ' ' << arr[i].summa << endl;
            }
        }
    }
    else {
        tmp = (k + 1) % n;
        while (tmp != k && !found) {
            if (hashtable[tmp].FIO == a) {
                found = true;
                cout << "ответ найден в позиции: " << tmp << endl;
                found2 = FALSE;
                for (i = 0; i < n && !found2; i++) {
                    if (arr[i].FIO == a) {
                        found2 = TRUE;
                        cout << arr[i].FIO << ' ' << arr[i].account << ' ' << arr[i].summa << endl;
                    }
                }
            }
            tmp = (tmp + 1) % n;
        }
    }
    if (found == 0) {
        cout << "человек не найден" << endl;
    }
}

void DelEL(client* arr, int n) {
    string fio;
    int pos;
    bool found = 0;
    int temp;
    if (arr == nullptr) {
        cout << "Ошибка";
        return;
    }
    else {
        cout << "введите ФИО для удаления: ";
        cin.ignore();
        getline(cin, fio);
        pos = hash_function(fio, n);
    }
    if (fio == arr[pos].FIO) {
        found = 1;
        strcpy(arr[pos].FIO, "");
        arr[pos].account = 0;
        arr[pos].summa = 0;
    }
    else {
        temp = (pos + 1) % n;
        while (temp != pos && !found) {
            if (fio == arr[temp].FIO) {
                found = 1;
                strcpy(arr[temp].FIO, "");
                arr[temp].account = 0;
                arr[temp].summa = 0;
            }
            temp = (temp + 1) % n;
        }
    }
    if (found == 0) cout << "человек не найден" << endl;
    else cout << "запись удалена" << endl;
}

int main()
{
    int choice = -1;
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    srand((int)time(NULL));
    int  n = 50;
    client* arr = new client[n];
    hashtab* hashtable = new hashtab[n];
    while (choice != 0) {
        cout << "1 заполнение массива структур" << endl;
        cout << "2 сохранение в файл" << endl;
        cout << "3 загрузка из файла" << endl;
        cout << "4 добавление элемента в массив" << endl;
        cout << "5 создание и печать хэш таблицы" << endl;
        cout << "6 поиск ФИО в хэш таблице" << endl;
        cout << "7 удаление заданного ФИО" << endl;
        cout << "8 печать массива" << endl;
        cout << "0 выход из программы" << endl;
        cout << "введите пункт меню: ";
        cin >> choice;
        cout << endl;
        switch (choice) {
        case 1:
            makearr_rnd(n, arr);
            break;
        case 2:
            SaveArr(arr, n);
            break;
        case 3:
            LoadArr(arr, n);
            break;
        case 4:
            addEl(arr, n);
            break;
        case 5:
            hashta(arr, n, hashtable);
            break;
        case 6:
            hashFound(hashtable, n, arr);
            break;
        case 7:
            DelEL(arr, n);
            break;
        case 8:
            for (int j = 0; j < n; j++) cout << arr[j].FIO << " " << arr[j].account << " " << arr[j].summa << endl;
            break;
        case 0:
            return 0;
        }
    }
    return 0;
}