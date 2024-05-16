#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef struct node{
	char *userName;   //isim
	int deleted;     //silinmisse 1 silinmemise 0 degerini tutar
}Node;

/*
@brief h1() adlý hash fonksiyonu, Horner Kuralina göre elde edilen anahtar deger asal sayinin modunu alarak integer deger donduruyor.

@param userName, kullanicinin girdigi isim. isimden anahtar deger elde ediliyor
@param m, asal sayi
@return anahtar degerinin asal saýyaya gore modu aliniyor.
*/
int h1Eval(char *userName, int m){
	int i, j, hash;			//hash: h1() fonksiyonun ciktisi olacak
	j = i = hash = 0;		//i,j: userName indeksi
	while(userName[j] !='\0'){
		//Horner Kurali: userName[0]*31^0 +  userName[1]*31^1 +...+userName[n-1]*31^(n-1)
		//Normalde anahtar degeri hesaplanip en son mod almak yerine her karakterde mod alýp toplayarak ilerlemek daha mantikli.
		//Horner kuralina baktigimiz zaman modunu almadan ilerlersek cok buyuk sayýlar cýkýyor.
		hash = ( hash + ( (int)( pow( (31 % m) , i++) ) * (userName[j++] % m)  ) % m ) % m;
	}
	return hash;
}

/*
@brief h2() adlý hash fonksiyonu, Horner Kuralina göre elde edilen anahtar deger asal sayinin modunu alarak ve sonda 1 ekleyerek integer deger donduruyor.

@param userName, kullanicinin girdigi isim. isimden anahtar deger elde ediliyor
@param m, asal sayi
@return anahtar degerinin asal saýyaya gore modu aliniyor.
*/
int h2Eval(char *userName, int m2){
	int i,j, hash;   //hash: h2() fonksiyonun ciktisi olacak
	j = i = hash = 0;  //i,j: userName indeksi
	while(userName[j] !='\0'){
		//Horner Kurali: userName[0]*31^0 +  userName[1]*31^1 +...+userName[n-1]*31^(n-1)
		//Normalde anahtar degeri hesaplanip en son mod almak yerine her karakterde mod alýp toplayarak ilerlemek daha mantikli.
		//Horner kuralina baktigimiz zaman modunu almadan ilerlersek cok buyuk sayýlar cýkýyor.
		hash = ( hash +  (int)( pow( (31 % m2) , i++) ) * (userName[j++] % m2)  ) % m2;
	}
	return hash +1;  //elde edilen anahtar degerinin asal sayýya gore modu alinip 1 eklendi
}

/*
@brief h() adlý hash fonksiyonu, (h1(key) + i*h2(key)) % m degerini hesaplar.

@param userName, kullanicinin girdigi isim. isimden anahtar deger elde ediliyor
@param m, asal sayi
@param i, hesaplamak icin verilen adim sayisi
@return yukarida verilen denklemin sonucu dondurulur
*/
int hEval(char *userName, int m, int i){
	return ( h1Eval(userName, m) + i*h2Eval(userName, m-2) ) % m; 
}

/*
@brief 3 islem icin hizmet eden bir fonksiyon
		1)Eger aranan deger bulunmussa onun indeksini dondurur
		2)Eger eleamn eklemek istiyorsan onun icin uygun bir indeks dondurur
		3)Silmek istiyorsan onun icin uygun bir indeks dondurur.
@param hashTable, isimlerin saklandigi tablolar
@param name, aranan isim
@param m, asal sayi/Tablo boyutu
@return degerin islemine gore dondurdugu indeks
*/

int searchUserName(Node *hashTable, char *name, int m){
	int i = 0;
	int hashValue = hEval(name, m, i);   //hesaplanip donen indeks
	int first = m;						//first degiskeni eger ben eleman eklemek istiyorsam onun icin uygun indeks degerini tutmam gerekecek
										//Tabi once gerekli kontrollerle arama  yaptiktan sonra bu degeri dondurebilirim.
										
		//arama yaparken oncelikle hashValue indeksindeki isimle karsilastirma yapmam gerekecek
		// eger ayni ise ismimi bulmus olabilme ihtimalim var. net degil
		// i < m kontrolu de sonsuz donguye girmemesi icin.  
		while( strcmp( hashTable[hashValue].userName , name ) != 0   && i < m){
			
			//Buradaki kontrol eger herhangi bir deger yoksa demek ki bu indekste daha once hic ekleme ya da cikartilma yapilmamis
			//Ben de arama yapýyorum demek ki buraya herhangi bir ugrayan olmamýssa bundan sonraki indekslerde aranilan deger yok.
			if( hashTable[hashValue].userName[0] == '\0' ){
				
				//Buradaki kontrol daha cok ekleme yapabilmek icin donen deger
				if(first == m) //first degiskeni atandigi degiskeni esit ise demek ki ilk baktigi indekse kimse ugramamis ve direkt hesaplanan indeks doner
					return hashValue;
				return first; // first degiskeni atandigi degerden farkli olduguna gore baska indekslerdeki degerlerle karsilastirma yapilmis
							//En sonunda da ilk uygun indeks donulur.
			}
			
			//Buradaki kontrol uygun ilk yere ekleme yapabilmek ya da silinen deger icin var
			//silinmisse demek ki fiziksel olarak var olan indeksi dondurecek
			//ekleme icin ise eger ileride aranan deger yoksa bu ilk uygun indekse yerlestirmek icin kullanilacak.
			//arama icinse pozitif deger donse bilesikinti olusmaz.
			//asil yerde icerikleri karsilastirsam ayni olmadigini gorurse demek ki yine yok diyebilirim
			if(hashTable[hashValue].deleted && first == m)
				first = hashValue;
			hashValue = hEval(name, m, ++i);
		}
		if(i >= m)    //tablo agzýna kadar dolu oldugu icin tablo boyutu kadar deger donecek
			return first;
		if(hashTable[hashValue].deleted) //Yukarida net degil yazmistim.Cunku o deger fiziksel olarak olabilir ama silinmisse daha onceden aslinda yok
			return ( hashValue * -1) - 2; // kendisine ozgu bir formulle donduruyorum
										//Eger tablomda silinmis eleman varsa eklemek yerine deleted = 0 yapmam gerekecek
										// Ben de bu donen deger ile ileride gerekli sureclerden sonra fiziksel olarak bulundugu konumu bulabiliyorum.
		else //bu adima kadar geldigine gore tabloda girilen isim var ve indeksini dondurme vakti
			return hashValue;
}

/*
@brief hash tablosuna eleman eklemek icin var

@param hashTable, isimlerin tutuldugu tablo
@param name, eklenecek isim
@param adress, tablodaki hangi indekse eklenecek

return;
*/
void createNode(Node** hashTable, char* name, int adress){
	Node * tmp = *hashTable;	
	tmp[adress].userName = (char*)malloc(sizeof(char) * (strlen(name)+1) ); //gerekli gözdeki userName degiskeni icin yer acilir
	strcpy(tmp[adress].userName, name);										//acilan yere name degiskenini kopyaliyoruz
	tmp[adress].deleted = 0; 												//deleted degiskenini 0 yapioruz cunku sadece fiziksel olarak yok tabloda
}

/*
@brief tabloya eleman ekleniyor

@param hashTable, isimlerin tutuldugu tablo
@param name, tabloya eklenecek isim
@param m, tablo boyutu

return eklendiyse eger hangi indekse eklendi ya da eklenmediyse negatif deger doner
*/
int addUserName(Node** hashTable, char *name, int m){
	int hashIndex = searchUserName(*hashTable, name, m);
	int hashValue;
	
	if(hashIndex == m) 			//Tablo boyutu kadar donerse demek ki eklenecek bos alan yok
		return -2;
	if(hashIndex >= 0){			//pozitif bir deger donerse ya eleman var ya da eklenecek ilk uygun indeks var.
		if(strcmp(name, (*hashTable)[hashIndex].userName) == 0 ) //eleman varsa eklenecek bir durum da ortada kalmiyor
			return -1;
		hashValue = hashIndex;   
		createNode(hashTable, name, hashValue);   //burada ilk uygun indekse eleman eklenir.
	}
	if(hashIndex < -1){  //Burada -1'den ise demek ki tabloda sadece fiziksel olarak var olan degerin deleted degiskenini 0 yapacaz
		hashValue = ( hashIndex * -1) - 2;
		(*hashTable)[hashValue].deleted = 0;
	}
	return hashValue;  //Hangi indekse eklendiyse o deger doner
}

/*
@brief tablodan eleman siliniyor

@param hashTable, isimlerin tutuldugu tablo
@param name, tablodan silinecek isim
@param m, tablo boyutu

return silindiyse eger hangi indekse eklendi ya da eklenmediyse farkli nedenlerden dolayi negatif deger doner
*/
int deleteUserName(Node **hashTable, char *name, int m){
	int hashIndex = searchUserName(*hashTable, name, m);
	if(hashIndex == m) // Demek ki silinecek eleman yok
		return -1;
	
	//hashIndex eger sifirdan kücükse demek ki o eleman yok
	//searchUserName fonksiyonu bir de ekleme yapabimek icin uygun indeks degerini donduruyor
	//eger name ile o indeksteki eleman ayni degilse name degiskeni de tabloda yok
	if(hashIndex < 0 || strcmp ( name ,(*hashTable)[hashIndex].userName ) != 0)
		return -1;
	(*hashTable)[hashIndex].deleted = 1; // var olan ismin deleted degerini 1 yapýyoruz. Tablodan sildik ama fiziksel olarak var
	return hashIndex;	//Hangi indeks silinmis onun degeri doner
}

/*
@brief tablonun icindeki degerleri terminale yazar

@param hashTable, isimlerin tutuldugu tablo
@param m, tablo boyutu
*/
void print(Node* hashTable, int m){
	int i;
	printf("\tHashTable Degerler\n");
	for(i = 0; i < m; i++){
		if(hashTable[i].deleted == 0)
			printf("adress: %d name: %s\n", i, hashTable[i].userName);
	}
}

/*
@brief 3 islem icin adim adim neler yapmis ekrana basar
@param hashTable, isimlerin saklandigi tablolar
@param name, aranan isim
@param m, asal sayi/Tablo boyutu

@return
NOT: searchUserName() fonksiyonu ile ayni mantikta calisiyor
*/
void debugModPrint(Node* hashTable, char *name ,int m){
	int i = 0;
	int h1 = h1Eval(name, m);
	int h2 = h2Eval(name, m-2);
	printf("\tAdimlar\n");
	printf("h1(%s) = %d\n", name, h1); printf("h2(%s) = %d\n", name, h2); printf("h(key, i) = (h1(key) + i*h2(key)) mod m, i = 0\n");
	int hashValue = hEval(name, m, i);
	int first = m;
		//name ismiyle hashValue indeksindei deger ayni mi diye kontrol ediliyor.
		while( strcmp( hashTable[hashValue].userName , name ) != 0   && i < m){
			printf("%d. adreste %s kelimesi bulunamadi. (key : %d, user name: %s deleted: %d; i = %d)", hashValue, name, hashValue, hashTable[hashValue].userName, hashTable[hashValue].deleted,i);
			if( hashTable[hashValue].userName[0] == '\0')
				printf(" Bu adreste herhangi eleman yok.");
			printf("\n");
			if( hashTable[hashValue].userName[0] == '\0'){
				if(first != m) // first degiskeni atanan ilk degerle farkli ise demek ki olusan durum baska bir indkeks daha uygun
					hashValue = first;
				printf("%d. adres donus yapacak. (user name: %s deleted: %d; i = %d).\nUygun islem gerekli kontrol dogrultusunda yapilir/yapilmaz.\n", hashValue, hashTable[hashValue].userName, hashTable[hashValue].deleted,i);
				return;
			}
			
			//Buradaki kontrol uygun ilk yere ekleme yapabilmek ya da silinen deger icin var
			//silinmisse demek ki fiziksel olarak var olan indeksi dondurecek
			//ekleme icin ise eger ileride aranan deger yoksa bu ilk uygun indekse yerlestirmek icin kullanilacak.
			if(hashTable[hashValue].deleted)
				first = hashValue;
			hashValue = hEval(name, m, ++i);
		}
		if(i >= m)  //tablo agzýna kadar dolu oldugu icin tablo boyutu kadar deger donecek
			printf("%d. adres donus yapacak. Uygun islem gerekli kontrol dogrultusunda yapilir.\n", i);
		if(hashTable[hashValue].deleted){
			//deger fiziksel olarak olabilir ama silinmisse daha onceden aslinda tabloda yeri yok
			// kendisine ozgu bir formulle donduruyorum
			//Eger tablomda silinmis eleman varsa eklemek yerine deleted = 0 yapmam gerekecek
			// Ben de bu kendine ozgu formulle cikan deger ile ileride gerekli sureclerden sonra fiziksel olarak bulundugu konumu bulabiliyorum.
			printf("%d. adres donus yapacak. (user name: %s deleted: %d; i = %d).\nUygun islem gerekli kontrol dogrultusunda yapilir/yapilmaz.\n", ( hashValue * -1) - 2, hashTable[hashValue].userName, hashTable[hashValue].deleted,i);
			printf("Arama ve silme islemi gerceklecesekse basarisiz olur. Silinmis bir isim eklenecekse basarili olur\n");
		}
		else //bu adima kadar geldigine gore tabloda girilen isim var
			printf("%d. adres donus yapacak. (user name: %s deleted: %d; i = %d).\nUygun islem gerekli kontrol dogrultusunda yapilir/yapilmaz.\n", hashValue, hashTable[hashValue].userName, hashTable[hashValue].deleted,i);
		
		return;
	
}

/*
@brief tablonun ilk halini olusturur

@param m , tablonun boyutu
@return tablonun adres degeri doner
*/
Node* initilazitionHashTable(int m){
	int i;
	Node* hashTable = (Node*)malloc(sizeof(Node)*m); //Tablo boyutu kadar yer acilir
	for(i = 0; i < m; i++){
		hashTable[i].userName = "";  //userName degiskenine bos eleman atanir
		hashTable[i].deleted = 1;  //deleted degiskeni 1 yapilir. Eleman olmadigi icin bos sayabiliriz
	}
	return hashTable;  //tablonun adres degeri doner
}

/*
@brief eski tablodaki degerleri(sadece fiziksel olarak bulunanlar disinda) yeni tabloya aktarir

@param hashTable, isimlerin saklandigi tablolar
@param m , tablonun boyutu
@return yeni tablonun adres degeri doner
*/
Node* reformHashTable(Node* hashTable, int m){
	Node* newHashTable = initilazitionHashTable(m); // yeni tablonun ilk hali
	int i, adress;
	printf("\tTablo Duzenlenmesi\n");
	for(i = 0; i < m; i++){
		if( hashTable[i].userName[0] == '\0'){  //daha onceden herhangi bir islem yapimamissa yeni tabloya da bir sey yapmýyoruz 
			printf("%d. adreste daha onceden herhangi bir isim eklenmemis ve yokmus\n", i);
		}
		else if(hashTable[i].deleted == 1){ //silindigi icin sadece fiziksel olarak var ondan yeni tabloya ekleme yapmiyoruz
			printf("%s ismi daha onceden %d. adreste bulunuyormus ama silindigi icin yeni tabloda yer alamaz.\n", hashTable[i].userName, i );
		}
		else{ //ekleme yapabilirz
			adress = addUserName(&newHashTable, hashTable[i].userName, m);
			if(adress == i) // donen degerler ayniysa demek ki eski ve yeni tablodaki degerler ayni olacak
				printf("Eski tablodaki ve yeni tablodaki yeri ayni. (adres: %d, isim: %s)\n", adress, hashTable[adress].userName);
			else // eski tabloya gore yeri farkli
				printf("Eski tablo: (adres: %d, isim: %s) ve yeni tablo: (adres: %d, isim: %s)\n", i, hashTable[i].userName, adress, newHashTable[adress].userName);
		}
		
	}
	//free(hashTable);
	return newHashTable; // yeni tablonun adres degeri doner
}

//@brief Bir sayinin asal olup olmadigini kontrol eden fonksiyon
//@param num, asal sayi olup olmadigi merak edilen sayi
//@return asal mi degil mi
int isPrime(int num) {
    if (num < 2) {
        return 0; // 2'den küçük sayilar asal degildir
    }
	int i;
    for (i = 2; i * i <= num; i++) {
        if (num % i == 0) {
            return 0; // Asal degil
        }
    }

    return 1; // Asal
}

//@brief Bir sayinin üstündeki en küçük asal sayiyi bulan fonksiyon
//@param num, asal sayinin alt siniri
//@return num degerinden sonra gelen ilk asal sayi
int findNextPrime(int num) {
    while (!isPrime(num)) {
        // Bir sonraki sayiyi kontrol et
        num++;
    }
    return num;
}

int main(){
	int m, N, adress, control2, control1 = 1;
	float loadFactor;
	do{
		printf("Load Factor: ");
    	scanf("%f", &loadFactor);
	}while(loadFactor >= 1 || loadFactor <= 0);
    printf("Kac tane giris olacak: ");
    scanf("%d", &N);
	printf("Tablonun boyutu(Onerilen = %d): ", findNextPrime( (int)ceil( N / loadFactor ) ) );
    scanf("%d", &m);
	char userName[50];
	Node* hashTable = initilazitionHashTable(m);
	while(control1){
		printf("\tIslemler\n1)Ekleme\n2)Silme\n3)Arama\n4)Tabloyu duzenlemek\n5)Tabloyu Listele\n6)Cikis\n"); scanf("%d", &control1);
		if(control1 < 4){
			do{
				printf("1)Normal Mod\n2)Debug Mod\n"); scanf("%d", &control2);
			}while(control2 < 1 || control2 > 2); //Baska deger girilmesin diye yapilan kontrol
			printf("Ismi giriniz: "); scanf("%s", userName);
		}
		switch(control1){
			case 1: //eleman eklemek
				if(control2 == 2) // debug mod icin ekrana yazdirilir
					debugModPrint(hashTable, userName, m);
				adress = addUserName(&hashTable, userName, m); //donen indeks deger
				if(adress >= 0)
					printf("%s ismi %d. adrese eklendi\n", userName, adress);
				else{ //negatif donerse demek ki eklenememis
					printf("%s ismi eklenemedi. ", userName);
					if(adress == -1) // mevcut oldugu icin eklenememis
						printf("Kullanici ismi(%s) tabloda mevcuttur\n", userName);
					else if(adress == -2) // tablo tamamen dolu
						printf("Tablo tamamen dolu oldugu icin islem gerceklesemiyor...\n");
				}
				break;
			case 2: //eleman silmek
				if(control2 == 2)
					debugModPrint(hashTable, userName, m);
				adress = deleteUserName(&hashTable, userName, m);
				if(adress >= 0)
					printf("%s ismi %d. adresten silindi.\n", hashTable[adress].userName , adress);
				else
					printf("%s ismi silinemedi.Olmayan isim silinemez\n", userName);
				break;
			case 3: // eleman aramak
				if(control2 == 2)
					debugModPrint(hashTable, userName, m);
				adress = searchUserName(hashTable, userName, m);
				if(adress == m || adress < 0) // adress degeri 0'dan kücük veya tablo uzunluguna esitse eleman yok
					printf("%s ismi bulunamadi\n", userName);
				else if(strcmp(hashTable[adress].userName, userName) != 0) // donen deger eger esit deilse degerle demek ki yok
					printf("%s ismi bulunamadi\n", userName);
				else
					printf("%s ismi %d. adreste bulundu.\n", hashTable[adress].userName , adress);
				break;
			case 4: // yeniden duzenleme
				hashTable = reformHashTable(hashTable, m);
				break;
			case 5: //tabloyu yazdirma
				print(hashTable, m);
				break;
			case 6: //cikis
				control1 = 0;
				break;
			default:
				printf("Gecerli komut giriniz...\n");
		}
	}
	
	free(hashTable);
	return 0;
}
