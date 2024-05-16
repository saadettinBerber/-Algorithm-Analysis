#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

/*
@brief Longest Common Sequence matrisinin icinde edindigimiz bilgilerle en uzun ifade/ifadeler dizi icinde saklanilir.
@param string2, kullanici tarafindan girilen 2. ifade
@param result, adim adim en uzun ifadeye uygun karakterler eklenir ve bu degiskende saklanir
@param tableSelect, icinde hangi indekslerde ayni karaktere denk gelirse 1 bilgisi saklanilir
@param table, kucuk problemlerden buyuk problemi elde etmeye calisirken edilen bilgiler var
@param k, table matrisinde gezerken satiri temsil eder
@param l, table matrisinde gezerken sutunu temsil eder
@param index, uzun ifadeyi olustururken kalinan yeri tutuyor. Ilk atana deger en uzun ifadenin uzunlugu kadar
@param results, uzun ifade/ifadeler matrise kaydedilir.
return
*/
void findSecret(char *string2, char *result, int **tableSelect , int **table , int k, int l, int index, char ***results){
	int i;
	if(index <= 0){    										//en uzun ifade bulunmus olunuyor. Simdi onu results matrisine kaydetmek kaliyor
		i = 0;
		while(( *results) [i][0] != '\0'){ 					//results matrisinin boyutu hep degisecegi icin eger ilk karakter '\0' ise o zaman ondan sonra icinde deger yok.
			if(strcmp( ( *results)[i], result) == 0 )		//Eger daha önce eklenen karakterden varsa daha önce eklendigi icin fonksiyon sonlandirilir.
				return;
			i++;
		}
		strcpy( (*results) [i], result);									//Bu kod satirina gelindigine göre uzun ifade daha once eklenmemis. Gerekli alana eklendi
		(*results)  = (char**)realloc( (*results), sizeof(char*) * (i+3));	//Bir dahaki uzun ifadeye ekleme yapabilmek icin yer acilmasi gerek.
		(*results) [i+2] = (char*) malloc( (strlen(result) + 1) * sizeof(char)); //Yeni acilan goze yer acilmasi gerek.
		(*results) [i+2][0] = '\0';											//Results dizisinde gezme sirasinda durabilmek icin yeni eklenen indekse '\0' karakteri eklenir.
		return;
	}
	
	if(tableSelect[k][l] == 1){
		result[ index - 1 ] = string2[k-1];												//Eger tableSelect 1 ise demek ki ayni harfler denk gelinmis o zaman result degiskenine
		findSecret(string2, result, tableSelect, table, k-1, l-1, index -1, results);	// sondan ekleme yapilir. Sonra da hem satir hem de sutun degerleri bir azaltilarak fonksiyon cagrisi yapilir.
	}																					//Ekleme yapildigi icin indeks degeri de 1 azaltilir
	
	else if(table[k-1][l] == table[k][l-1]){											//Eger alt ve ust degerler esitse gidebilecegi 2 farkli yol. Burada ikiye ayrilip ayri	
		char *tmp = (char*)malloc( (strlen(result) +1) * sizeof(char) );				//ayri bulmasi gerek. 
		strcpy(tmp,result);																//2 farkli yol olacagi icin tmp degiskeni olusturulur. Result degiskeninin icerigi kopyalanir
		findSecret(string2, tmp, tableSelect, table, k-1, l, index, results);			//Ustten gidilir burada
		findSecret(string2, result, tableSelect, table, k, l-1, index, results);		//Soldan gidilir kisimdan 
		return;
	}
	else if( table[k-1][l] > table[k][l-1])												//Sol taraftaki deger daha buyuk oldugu icin soldan gidilir
		findSecret(string2, result, tableSelect, table, k-1, l, index, results);		//Sol taraftan gitmek demek o anda bulunan sutun indeksini 1 azaltmak demek
	
	else if(table[k-1][l] < table[k][l-1])												//Ust taraftaki deger daha buyuk oldugu icin Ust gidilir
		findSecret(string2, result, tableSelect, table, k, l-1, index, results);		//Ust taraftan gitmek demek o anda bulunan satir indeksini 1 azaltmak demek
	
	return;
}

/*
@brief Longest Common Sequence matrisinin kullanici tarafindan anlasilir sekilde yazdirilmasi
@param table, kucuk problemlerden buyuk problemi elde etmeye calisirken edilen bilgiler var
@param string1, kullanici tarafindan girilen 1. ifade
@param string2, kullanici tarafindan girilen 2. ifade
@N1, string1 ifadesinin uzunlugu
@N2, string2 ifadesinin uzunlugu
return 
*/
void print(int **table, char * string1, char * string2, int N1, int N2){
	int i, j;
	printf("    ");
	for(i  = 0 ; i < N1; i++)
		printf("%c ", string1[i]);
	printf("\n  ");
	for(i = 0; i < N2; i++){
		if(i != 0 && i < N2)
			printf("%c ", string2[i-1]);
		for(j = 0; j < N1; j++)
			printf("%d ",table[i][j]);
		printf("\n");
	}
	printf("\n");
}

/*
@brief Harf sekans matrisinin kullanici tarafindan anlasilir sekilde yazdirilmasi
@param tableSelect, kucuk problemlerden buyuk problemi elde etmeye calisirken edilen bilgiler var
@param string1, kullanici tarafindan girilen 1. ifade
@param string2, kullanici tarafindan girilen 2. ifade
@N1, string1 ifadesinin uzunlugu
@N2, string2 ifadesinin uzunlugu
return 
*/
void printLetterSequence(int **tableSelect, char * string1, char * string2, int N1, int N2){
	int i, j;
	printf("      ");
	for(i  = 0 ; i < N1; i++)
		printf("%c  ", string1[i]);
	printf("\n  ");
	for(i = 0; i < N2; i++){
		if(i != 0 && i < N2)
			printf("%c ", string2[i-1]);
		printf(" ");
		for(j = 0; j < N1; j++){
			if(tableSelect[i][j] == 1)				//Ayni eleman denk geldiyse o zaman çaprazdan kalitim alir.
				printf("C  ");
			else if(tableSelect[i][j] == 3)			//3 numara demek iki farkli ifade olabilir anlamina denk geliyor ve bunlarin uzunlugu esit
				printf("US ");
			else if(tableSelect[i][j] == -1)		//-1 numara ustten kalitim aliyor.
				printf("U  ");
			else if(tableSelect[i][j] == 2)			//2 numara demek soldan kalitim aliyor.
				printf("S  ");
			else
				printf("_  ");
		}
		printf("\n");
	}
	printf("\n");
}

/*
@brief elimizdeki 2 integer sayidan maksimum olanini bulmak
@param x, karsilastiralacak ifadelerden biri
@param y, karsilastiralacak ifadelerden biri
return maksimum deger
*/
int max(int x, int y){
	if(x > y)
		return x;
	return y;
}

int main(){
	char string1[50];
	char string2[50];
	printf("1. ifadeyi giriniz: "); scanf("%s", string1);   
	printf("\n2. ifadeyi giriniz: "); scanf("%s", string2);
	int N1 = strlen(string1)+1;
	int N2 = strlen(string2)+1;
	int i,j, mark;
	
	int **table = (int**) calloc(N2, sizeof(int*));			//Long Common Sunsequence matrisi
	int **tableSelect = (int**) calloc(N2, sizeof(int*));	//Adim adim harf sekansi matrisi
	//matrisi tasarlarken 2. ifade satir olarak dusunuldu. sutun ise 1. ifade olarak dusunuldu.
	for(i = 0; i < N2; i++){								//Gerekli alanlar acilir
		table[i] = (int*)calloc(N1, sizeof(int));
		tableSelect[i] = (int*)calloc(N1, sizeof(int));
	}
		
	printf("Matrislerin Ilk Hali\n");
	print(table, string1, string2, N1, N2);				//Matrislerin ilk hali
	printLetterSequence(tableSelect, string1, string2, N1, N2);
	//Ic ice for donguleri ile Long Common Sebsequence matrisi doldurulur.
	for(i = 1; i < N2; i++){							//satirdaki degerler 2. ifadeye ait oldugu icin N2'ye kadar gidilir.
		printf("%d. satir sonu matrisler\n", i);		
		for(j = 1; j < N1; j++){									//sutundaki degerler 1. ifadeye ait oldugu icin N1'e kadar gidilir
			if( string2[i-1] !=  string1[j-1]){						//Iki karakter degeri esit degilse table matrisinin icinde hangi 
				table[i][j] = max ( table[i-1][j],  table[i][j-1]);	//indeks daha buyukse o yazilir. Amacimiz en uzun sekansi bulmak cunku
				if(table[i-1][j] == table[i][j-1])					//Birbirine esitse iki yoldan da gelinebilir
					tableSelect[i][j] = 3;
				else if(table[i-1][j] > table[i][j-1])				//Ustteki deger daha buyukse oradaki alt problem ele alinacak
					tableSelect[i][j] = -1;
				else if(table[i-1][j] < table[i][j-1])				//Soldaki deger daha büyükse oradaki alt problem kalitim olarak alinir.
					tableSelect[i][j] = 2;
			}	
			else{
				table[i][j] = table[i-1][j-1] + 1;					//Esit ise bir üst sol carpraz indekse bakilir. Cunku esit ise bir onceki
				tableSelect[i][j] = 1;								//alt problemde ne kadar uzunlukta bulunmus bizim icin o onemli oluyor.
			}
		}
		print(table, string1, string2, N1, N2);						//Tablolar yazdiriliir
		printf("S: SoldaN kalitim alinmis.\nU: Ustten kalitim alinmis.\nUS: Ustten ya da soldan kalitim alinmis.\nC: Caprazdan alinmis.\n");
		printLetterSequence(tableSelect, string1, string2, N1, N2);
	}
	
	int length = table[N2-1][N1-1];							//Alt problemleri kullanarak nihai en uzun ifadenin uzunlugu elde edildi ve matrisin en sag altinda
	printf("En uzun ifadenin boyutu: %d\n", length);
	
	char **results = (char**) malloc ( 2 * sizeof(char*) );	//Simdi ifadeleri bulmak icin results matrisinde tutulacak. 
															//Tekrar olan ifadeler bastirilmamasi icin matriste saklanilacak.
	results[0] = (char*) malloc( (length + 1) * sizeof(char));//Gerekli alanlar acildi matris icin
	results[1] = (char*) malloc( (length + 1) * sizeof(char));
	
	char *result2 = (char*)malloc( (length+1) * sizeof(char)); //Matrisin en sag altindan baslayip en uzun ifade bulunulmak istenilecek.
																//Ondan dolayi eklemeler yapilacak. Her uygun adimda. Elimizde dizinin boyutu da var zaten
	
	for(i = 0; i< length; i++)								//Debug yaparken daha kolay oluyordu. Ondan dolayi nokta ile dolduruldu
		result2[i] = '.';									//Bir de malloc ile acarken arada dizinin ortasinda '\0' ifadesi olabiliyor ondan da gerekli
	result2[length] = '\0';
	
	results[0][0] = '\0';									//Diziye eklenen bir ifade daha once eklenilmeyecek ondan dolayi ben de 
	results[1][0] = '\0';									//  durma kosulu olusturdum. Ilk elemani '\0' ise dizi oraya kadarmis
	
	
	findSecret(string2, result2, tableSelect, table, N2-1, N1-1, length, &results); //Recursive sekilde uzun ifadeleri bulunur ve dizide saklanir
	
	
	i = 0;
	while(results[i][0] != '\0'){		//Son olarak results icinde kaydedilen farkli ifadeler yazdirilir.
		printf("%s\n", results[i]);
		free(results[i]);
		i++;
	}
	
	//SOn olarak da acilan yerler free edilir.
	free(results);
	free(result2);
	for(i = 0; i < N2; i++){
		free(table[i] );
		free( tableSelect[i] );
	}
	free(table);
	free(tableSelect);
	
	return 0;
}
