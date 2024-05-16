#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node{ //Duugm bilgilerinin tutuldugu yer
	int totalNode;
	int *nodes;
}Node;

typedef struct maxEdge{ //Silmek istedigimiz kenarin bilgisinin tutuldugu yer
	int total;			//Silinecek kenar sayisi
	int weight;			//kenarin agirligi
	int node1;			//1. dugum
	int node2;			//2. dugum
}MaxEdge;

typedef struct graph{ //Dosyadan okunan grafin bilgileri tutulur.
	int max;
	Node* adjacentList;		//komsuluk listesi
	int toplulukSayisi;		//ne kadar topluluk var
	int **weight;			//dugumler arasi kenar agirligi. Yani kenardan ne kadar siklikta gecilmis
}Graph;

typedef struct path{ //elde edilen yolun uzunlugu ve icerigi
	int path_len;
	int *paths;
}Path;
 
typedef struct toplululukpath{  //elde edilen yollarin bazilari topluluk. onlarin bilgisi saklanir.
	Path *path; 
	int toplulukSayisi;
	int flagLimit;
}ToplulukPath;


/*
@brief dosyadan okunan harfin 65 moduna bolup ona gore dizide islem yapmasini saglayacak
@param ascii , harfin integer karsiligi
return integer
*/
int turnIndex(int ascii){
	return ascii % 65;
}

/*
@brief dosyadan komsuluk lsitelerini okur
@return graph yapisi doner
*/
Graph* readFileInput() {
    FILE *file = fopen("input.txt", "r");                    //Hangi dosyadan okunacak
    Graph *graph = (Graph*)malloc(sizeof(Graph)); 			//Grafin icinde saklanilacak
    Node* totalNode = (Node*) malloc(sizeof(Node) *  100); 
    int ascii, i, j,totalAdjacent ,max = 0;
    char neighbors[100];                                //dosyadan okuma yaparken komsuluk listesi burada tutulur.
    char ss;											//hangi dugumun komsuluk listeleri okunacak
    if (!file) {
        perror("Dosya açilamadi");
        return NULL;
    }
    
    for(i = 0; i< 100; i++){
		totalNode[i].totalNode = 0;
		totalNode[i].nodes = NULL;
	}
	
    while(fscanf(file, " %c:%[^;];", &ss, neighbors) > 0){			//Dosyanin formatindan kaynakli
    	ascii = ss;
    	ascii = turnIndex(ascii);          						//diziler üzerinde islem yapilacak ondan 65 mod alinarak yapilacak
    	if(ascii > max)											//max ifadeyi de dosyanin icinde maksimum sayinin uzunlugu kadar olacak.
			max = ascii;
    	totalAdjacent = ( strlen(neighbors) - 1 ) / 2 + 1;      //dosyadan okurken ne kadar komsu sayisi var bunla ogreniriz.
		totalNode[ascii].totalNode = totalAdjacent;
		totalNode[ascii].nodes = (int*)malloc(totalAdjacent * sizeof(int));
		i = j = 0;
		while(j < totalAdjacent){								//burada komsuluk listesini doldurmaya baslariz
			if(neighbors[i] != ','){							
				totalNode[ascii].nodes[j] = turnIndex(neighbors[i]);
				j++;
			}
			i++;
		}
	}
	graph->adjacentList = realloc(totalNode, (max+1) * sizeof(Node) ); //graph yapima atama yapilir. Fazla alanlardan da kurtulunur.
	graph->max = max+1; max++;
	int **weight=(int**)calloc(max, sizeof(int*));
	for(i = 0; i < max;i++)
		weight[i] = (int*)calloc(max, sizeof(int));
	graph->weight = weight;
    fclose(file);
    return graph;
}

/*
@brief queue yapisi kullaniliyor. Ondan dolayi rear ve front degerlerini burada artirip uygun deger doner
@param value, harf degerinin 65 moda bolunmus hali
@param max, komsuluk listesinin uzunlugu
return rear ya da front degerinin 1 artirilmadan onceki deger donulur.
*/

int updateRearFront(int *value, int max){
	int tmp = *value;
	*value = (*value+1) % max;
	return tmp;
}

/*
@brief elimdeki dugumun diger tum dugumlere olan harita isisini bulur. 
Misal A dugumunden C dugumune giderken hangi dugumler uzerinden gecmis o bulunur.

@graph, dosyadan okunan graph yapisi
@param ascii, dugumun 65 mod degeri
return prev degeri ise bahsedilen dugumlere nasil gidildigine dair bilgi
*/

int* findPathBetweenNodes(Graph *graph, int ascii){
	int i,j , rear, tmpAscii, adjacentAscii ,front,max = graph->max;
	Node* adjacentList = graph->adjacentList;
	int *visited = (int*)calloc(max,sizeof(int));
	int *prev = (int*)calloc(max,sizeof(int));
	int *queue = (int*)calloc(max, sizeof(int));
	rear =  front = 0;
	
	for(i = 0; i < max; i++)   //Fonksiyon sonunda -1 icerigi varsa o dugumde demek ki o dugumden hic gecilmemis
		prev[i] = -1;
	
	tmpAscii = turnIndex(ascii);   //Gerekli donusum
	ascii = turnIndex(ascii);
	prev[ascii] = ascii;          //Gidilen yollar
	visited[ascii] = 1;			//Ugranilan dugume tekrar ugramasin diye. Parent dugumune gidilmesi engellenilir.
	queue[rear++] = ascii;		//queue veri yapim
	
	while( front != rear){		
		tmpAscii = queue[ updateRearFront(&front, max)];
		i = j = 0;
		while( i < adjacentList[tmpAscii].totalNode+j){				//BUrada queu yapisindan gelen elemanin komsularina bakiliyor.
			adjacentAscii =adjacentList[tmpAscii].nodes[i];			 //Ziyaret edilmis mi diye bakilir. 
			if( visited[ adjacentAscii ] == 0 && adjacentAscii >= 0){ //Eger edlimemisse daha onceden ugranmamis ondan queue yapisina eklenir
				visited[adjacentAscii] = 1;
				prev[adjacentAscii] = tmpAscii;						//Bahsedilen yol ya da iz
				queue[updateRearFront(&rear, max)] = adjacentAscii;//queue yapisina ekleme yapilir
			}
			if(adjacentAscii < 0)
				j++;
			i++;
		}
	}
	
	return prev;
}

/*
@brief var olan kisa yol yazdirilir
@path, elde edilen yolun veri yapisi. icerigi ve uzunluk bilgisi bulunur.
return
*/
void printPath(Path* path){
	if(path == NULL)
		return;
	int i;
	for(i =0; i < path->path_len; i++)
		printf("%c ", path->paths[i] + 65);
	printf("\n");
}

/*
@brief kenar silmek icin saklanilan veri yapisi initialize edilir.
@param i, 1. dugum
@param j, 2. dugum
@param max, en agir kenar degeri
return *maxEdge
*/
MaxEdge *createMaxEdgeInitial(int i, int j, int max){
	MaxEdge *maxEdge = (MaxEdge*)malloc(sizeof(MaxEdge) * 2);
	maxEdge[0].node1 = i;
	maxEdge[0].node2 = j;
	maxEdge[0].weight = max;
	maxEdge[0].total = 1;
	return maxEdge;
}

/*
@brief kenar silmek icin saklanilan veri yapisinin icerigi sadece. dizide saklanilir. Burada da dizinin sadece o elemani uretilir.
@param i, 1. dugum
@param j, 2. dugum
return maxEdge
*/
MaxEdge createMaxEdge(int i, int j){
	MaxEdge maxEdge;
	maxEdge.node1 = i;
	maxEdge.node2 = j;
	return maxEdge;
};

/*
@brief kenarlarin agirligi hesaplanir ve maksimum kenarlar bulunur.
@param graph, dosyadan okunan graph yapisi
return maxEdge
*/
MaxEdge *hesapla(Graph *graph){
	Node* adjacentList = graph->adjacentList;
	int i,j, ascii, tampon, total, maxEdgeNum = 0;
	MaxEdge *maxEdge = (MaxEdge*)malloc(sizeof(MaxEdge) * 2); // Birden fazla maksimum kenar olabilecegi icin yer acilir
	for(i = 0; i < graph->max; i++){			//tampon degeri komsuluk listesinden dugum silme silme -1 olarak kaydedilir. 
		j = tampon =0;							//Silinen dugum sayisi kadar donmesi gerek ondan tampon deger tutulur.
		while(j < adjacentList[i].totalNode + tampon){
			ascii = adjacentList[i].nodes[j];		//Komsulara bakilip agirlik matrisindeki bilgilerle toplanilir.
			if(ascii >= 0){							//Komsuluk matrisinden -1 degeri gelebilir silinirise
			total = graph->weight[i][ascii] + graph->weight[ascii][i];
			graph->weight[i][ascii] = graph->weight[ascii][i] = 0; //Bir daha toplamaya gerek yok ileride.
			if(total > maxEdgeNum){ // Eger yeni maksimum deger varsa o zaman var olan maks degerleri free edip yeniden ekleme yapmak lazim.
				free(maxEdge);
				maxEdge = createMaxEdgeInitial(i,ascii,total);
				maxEdgeNum = total;
			}
			else if(total == maxEdgeNum && total != 0){ // Var olan maks degere esitse demek ki birden fazla kenar silinecek. Ondan burada tutulur.
				maxEdge = realloc(maxEdge, sizeof(MaxEdge) * (maxEdge[0].total+2) );
				maxEdge[maxEdge[0].total] = createMaxEdge(i,ascii);
				maxEdge[0].total++;
				}
			}	
			else
				tampon++;
			j++;
		}
	}
	return maxEdge;
}

/*
@brief maks kenarlar bulunduktan sonra silme islemine geldi sira. var olan kenarlari  komsuluk listesinde -1 yapar
ve toplam komsu dugum sayilarini 1 azaltir.
@param graph, dosyadan okunan graph yapisi
return
*/
void deleteEdge(Graph *graph){
	int i,j, adjacentNum, node1, node2;
	MaxEdge *maxEdge = hesapla(graph); 
	for(i = 0; i < maxEdge[0].total; i++){
		node1 = maxEdge[i].node1;             //Silinecek kenarin dugumleri
		node2 = maxEdge[i].node2;
		adjacentNum = graph->adjacentList[node1].totalNode;
		for(j = 0; j < adjacentNum; j++){			//Komsuluk ilislisi kesilir
			if( graph->adjacentList[node1].nodes[j] == node2 )
				graph->adjacentList[node1].nodes[j] = -1;
		}
		
		adjacentNum = graph->adjacentList[node2].totalNode;
		for(j = 0; j < adjacentNum; j++){			//Komsuluk ilislisi kesilir
			if(graph->adjacentList[node2].nodes[j] == node1)
				graph->adjacentList[node2].nodes[j] = -1;
		}
		
		graph->adjacentList[node1].totalNode--;		//Toplam komsu sayilari azaltilir.
		graph->adjacentList[node2].totalNode--;
		printf("%c - %c kenari silindi\n", node1+65, node2+65);
	}
}
/*
@brief verilen isi ya da yol haritasi ile baslangic ve gidilmek istenilen nodun en kisa yolu bulunur
@param start, baslangic dugumu
@param end, gidilmek istenilen dugum
@max, graph yapisindaki komsluluk listesinin uzunlugu(Yani adjacentList uzunlugu)
retun bulunan yolun icerigi ve uzunlugu
*/
Path* findPath(int *prev, int start, int end, int max){
	if(start == end)
		return NULL;
	int *pathInclude = (int*)malloc(sizeof(int)* max);
	Path *path = (Path*)malloc(sizeof(Path));
	int tmpAscii, i=0;
	tmpAscii = end;
	while(tmpAscii != start){
		if(prev[tmpAscii] == -1)
			return NULL;
		pathInclude[i] = tmpAscii; i++;
		tmpAscii = prev[tmpAscii];
	}
	pathInclude[i] = start;
	pathInclude = realloc(pathInclude, sizeof(int)*(i+1) );
	path->paths = pathInclude;
	path->path_len=i+1;
	return path; 
}

/*
@brief kenar silindikten sonra olusan topluluk sayilarini bulmak icin kullanilir
@param graph, dosyadan okunan pragh yapisi
@prevs, her dugumun isi ya da iz haritasi
@param t, kullanicinin girdigi topluluk sayisi en az kaç olabilir onun sayisi
return topluluk sayisi, bulunan topluluklar yani kisa yollar, bir de t sayisina inilmis mi o doner
*/
ToplulukPath controlPath(Graph *graph, int** prevs, int t){
	int i,j, numberTopluluk, max = graph->max;
	ToplulukPath result;
	result.flagLimit = 0;
	Node* adjacenyList = graph->adjacentList;
	Path *toplulukPath = (Path*)malloc(sizeof(Path) * 2);  //Birden fazla topluluk olabilir ondan 2 kadarlik yer acilir.
	numberTopluluk = 0;
	for(i = 0; i < max; i++){
		if(adjacenyList[i].totalNode == 1){				//toplam node sayisi 1 ise topluluk olmak icin adaydir.
			j = i;
			while(j < max){
				Path *path = findPath(prevs[i], i,j,max);		//Kisa yol bulunur
				if( path != NULL && adjacenyList[ path->paths[0] ].totalNode == 1){ //NULL dönerse demek ki yol yokmus
					if(path->path_len <= t)											//yolun saonundaki dugum de toplam komsu dugum sayisi 1 ise
						result.flagLimit = 1;										// demek ki kesin olarak topluluk
					toplulukPath[numberTopluluk] = *path; numberTopluluk++;
					toplulukPath = realloc(toplulukPath, sizeof(Path) * (numberTopluluk+2) ); //Gerekli düzenlemeler yapilir.
				}
				j++;
			}
		}
	}
	result.toplulukSayisi = numberTopluluk;		//ne kadar topluluk var o bulunur.
	result.path = toplulukPath;					//Bulunan topluluk yollarin path veri tipinde dizisi
	return result;
}
/*
@brief agirlik matrisi yazdirilir
@param graph, dosyadan okunan graph yapisi
return
*/
void printWeight(Graph *graph){
	int i, j;
	printf("    ");
	for(i = 0; i < graph->max; i++)
		printf("%c  ", i+65);
	printf("\n");
	for(i = 0; i < graph->max; i++){
		printf("%c   ", i+65);
		for(j = 0; j< graph->max; j++){
			if(graph->weight[i][j] > 9 )
				printf("%d ", graph->weight[i][j]);
			else
				printf("%d  ", graph->weight[i][j]);
		}
			
		printf("\n");
	}
}

/*
@brief agirlik matrisi hesaplanilir. Bu da prevs matrisinden faydanilir. Geriye donuk bakarken artirim yapilir.
@param graph, dosyadan okunan Graph yapisi
@prevs, her dugumun isi/iz haritasi
*/
void hesaplaWeight(Graph *graph, int **prevs){
	int i,j, tmpAscii , ascii,max = graph->max;
	Node* adjacentList = graph->adjacentList;		//Komsluluk listesi bir degere atanir.
	for(i = 0; i < max; i++){						//Komsuluk listesinin uzunlugu kadar gezinme yapilir
		if(adjacentList[i].totalNode != 0){			//Toplam node sayisi 0 ise demek ki o dugum ya yok ya da ayrik bagimsiz.
			for(j = 0; j < max; j++){
				tmpAscii = j;						//Bu sefer bir tane dugumun isi/iz haritasini kullanarak tum kenarlarin agirligi bulunacak
				while(tmpAscii != i){						
					if( prevs[i][tmpAscii] >= 0){  //prevs matrisinin icindeki prev kulllanilarak artirilir.
						graph->weight[tmpAscii][ prevs[i][tmpAscii] ]++;
						tmpAscii = prevs[i][tmpAscii];			//Geriye donuk gider. Herhangi bir noktadan ilk noktaya yani baslangic noktasina gider.
					}
					else	//negatif ise zaten herhanhi bir yol yok.
						tmpAscii = i;
				}
			}
		}
	}
}

int main(){
	int tmpK, k, t; 							//Eger k tur üst üste topluluk sayisi ayni kalmissa sonlandir.
	int i,j, max, flag;								//Topluluklardan birinde üye sayisi minimum t oldugunda sonlandir.		
	int toplulukSayisi, iterasyon = 0;
	int **prevs;
	char *topluluk;
	ToplulukPath toplulukPath;
	Graph *graph = readFileInput();				//dosyadan okunulur.
	if(graph == NULL)
		return -1;
	max = graph->max;
	prevs = (int**)calloc( max+1, sizeof(int*));		//her dugumun isi/iz haritasinin saklandigi yer.
	for(i = 0 ; i <= max; i++)
		prevs[i] = (int*)calloc(max, sizeof(int));
		 
	//*********************************************
	
	flag = 1;
	
	do{
		printf("\nt sayisini girin: "); scanf("%d", &t);
		if(t <= 1)
			printf("1 ve 1'den kucuk olamaz t sayisi'");
	}while(t <= 1);
	
	printf("\nk sayisini girin: "); scanf("%d", &k);
	tmpK = k;
	
	printf("Baslangic Dugumlerin Komsu Sayisi\n");			//While dongusune girmeden once belli basli seylerin hesaplanmasi gerek.
	for(i = 0; i < max; i++){
			if(graph->adjacentList[i].totalNode != 0){
				printf("%c %d\n", i+65, graph->adjacentList[i].totalNode);
				prevs[i] = findPathBetweenNodes(graph, i);
			}	
		}
	hesaplaWeight(graph, prevs);
	printWeight(graph);

	while(flag && tmpK){
		printf("\t%d. iterasyon\n", ++iterasyon);graph->toplulukSayisi = 0;
		
		deleteEdge(graph);		//Hesaplanilan seyler üzerinden gerekli kenarlari siler
		printf("Dugumlerin Komsu Sayisi\n");
		for(i = 0; i < max; i++){
			if(graph->adjacentList[i].totalNode != 0){
				printf("%c %d\n", i+65, graph->adjacentList[i].totalNode);
				prevs[i] = findPathBetweenNodes(graph, i);
			}	
		}
		hesaplaWeight(graph, prevs);			//Sildikten sonra tekrardan agirlik hesaplanmasi gerek
		printWeight(graph);
		toplulukPath = controlPath(graph, prevs, t);
		//Bundan sonra belli basli seyler kontrol edilmeli.
		if( toplulukSayisi == graph->toplulukSayisi)
			tmpK--;
		else{
			tmpK = k;
			toplulukSayisi = graph->toplulukSayisi;
		}
			
		if(toplulukPath.flagLimit == 1)
			flag = 0;
	}
	//En sonunda yazdirilir.
	printf("**************************************\n");
	printf("Topluluk Sayisi: %d\n", graph->toplulukSayisi);
	for(i= 0; i < toplulukPath.toplulukSayisi; i++){
		printPath(&toplulukPath.path[i]);
	}
	
	return 0;
}
