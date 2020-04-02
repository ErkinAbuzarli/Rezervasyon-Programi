#include <stdio.h>   // Standart kütüphane
#include <time.h>   // Zaman kütüphanesi,bu programda yalnızca bu günün tarihini almak için kullanıyoruz.
#include <locale.h> // Türkçe karakterleri derleyicimize tanıtmak için kullandığımız kütüphane.

void Menu();                                            // Menüyü göstericez.
void Bilgiler();                                        // İstenildiğinde rezervasyon bilgilerini göstericez.
void Rezervasyon_ekle();                                // Yeni rezervasyon yapıcaz.
void Rezervasyon_kaldir();                              // Rezervasyon silicez.
void sil();                                             // Terminali temizliyicez.
void toplam_bilgiler_al();                              // Toplam bilgileri almak için kullanacağız.
struct tarih bugun_tarih();                             // Bugünün tarihini bulucaz.
struct oda yer();                                       // Rezervasyon tarihlerini dosyadan alıcaz.
int karsilastirma();                                    // Tarihleri karşılaştırıp büyük olanı bulmak için kullanıcaz.
int para_hesabi();                                      // Para hesabı yapmak için kullanıcaz.
int toplam_kazanc, toplam_rezervasyon, toplam_iptal;    // Toplam bilgiler için kullanıcaz.


struct tarih           
{                      
    int gun;           // Tarihleri gg.aa.yy olarak alıcaz.
    int ay;            // Her bir rezervasyonun ayrılıkta günü, ayı, yılı ve rezervasyon numarası 
    int yil;           // olacağı için "struct" la kolaylık sağlamış olacağız. Rezervasyon numarasını
    int rezerv_No;     // da sonra yaptığımız rezervasyon iptalini kolaylaştırmak için kullanacağız.
};

struct oda
{                                       // Odalar kayıtta işlem kolaylığından dolayı "char" olarak değilde, 
    int isim_num;                       // "int" olarak saklayacaz. Oda ismini(tipini) "isim_num" değişkeninde, 
    int num;                            // o oda tipinin kaçıncı odası olduğunu da "num" değişkeninde tutucaz.
    int rezervsayi;                     // "rezervsayi" kaç tane rezervasyon olduğunu tutacak. "rezervbaslangıc"
    struct tarih rezervbaslangic[10];   // rezervasyonun başlangıç tarihini, "rezervbitis" ise bitiş tarihini gösteriyor.
    struct tarih rezervbitis[10];       // "rezervbaslangıc" ve "rezervbitis" değişkenlerinin "struct tarih" tipinde 
};                                      // olmasının sebebi bir tarihi göstermesidir.

struct oda yer(int x){

    struct oda mekan[30];               // 30 tane oda olduğu için 30 yerlik "array" tanımlıyoruz.

    FILE *fyer;                         // oda.txt dosyasını okumak için "r" modunda açıyoruz.
    fyer = fopen("oda.txt", "r");

        for(int i=0; i<30; i++){        // Dosyadan her bir oda için bilgileri tek tek "fscanf" fonksiyonu ile alıyoruz.
            
            fscanf(fyer, "%d.%d - %d - %d.%d.%d - %d.%d.%d (%d)", &mekan[i].isim_num, &mekan[i].num, &mekan[i].rezervsayi, &mekan[i].rezervbaslangic[0].gun, &mekan[i].rezervbaslangic[0].ay, &mekan[i].rezervbaslangic[0].yil, &mekan[i].rezervbitis[0].gun, &mekan[i].rezervbitis[0].ay, &mekan[i].rezervbitis[0].yil, &mekan[i].rezervbaslangic[0].rezerv_No);            
            
            for (int j = 1; j <= mekan[i].rezervsayi; j++){     // Eğer rezervasyon sayısı 1 den çoksa onlarıda bu döngüde alıyoruz.
                
                fscanf(fyer, " | %d.%d.%d - %d.%d.%d (%d)", &mekan[i].rezervbaslangic[j].gun, &mekan[i].rezervbaslangic[j].ay, &mekan[i].rezervbaslangic[j].yil, &mekan[i].rezervbitis[j].gun, &mekan[i].rezervbitis[j].ay, &mekan[i].rezervbitis[j].yil, &mekan[i].rezervbaslangic[j].rezerv_No );
            }
        }

    fclose(fyer);

    return mekan[x];
}

struct tarih bugun_tarih(struct tarih bu){
    
    time_t rawtime;         // Değişkenleri tanımlıyoruz.
    struct tm *info;        

    time(&rawtime);
    info = gmtime(&rawtime ); 

    bu.gun = info->tm_mday;         // Bu günün, ayın, yılın tarihlerini alıp değişkenlere atıyoruz. Ay "array"-i
    bu.ay = info->tm_mon+1;         // 0-dan başlayarak saydığı için, örneğin ocak ayını 0-cı ay olarak gösteriyor. 
    bu.yil = info->tm_year-100;     // O yüzden +1 yaparak bunu düzeltiyoruz. Yil ise normalden 100 fazla gösterdiği için 
                                    // -100 yapıyoruz. (Zaman hesabını Unix zamanına göre yapıyoruz)
    return bu;
}

int karsilastirma(struct tarih x1, struct tarih x2){
    if (x1.yil > x2.yil){
        return 1;
    }else if (x1.yil < x2.yil){
        return 2;
    }else if (x1.yil == x2.yil){
        if (x1.ay > x2.ay){
            return 1;
        }else if (x1.ay < x2.ay){
            return 2;
        }else if (x1.ay == x2.ay){
            if (x1.gun > x2.gun){
                return 1;                  // Fonksiyonun 1 döndürmesi x1-in büyük olduğunu gösteriyor.
            }else if(x1.gun < x2.gun){
                return 2;                  // Fonksiyonun 2 döndürmesi x2-nin büyük olduğunu gösteriyor.
            }else if(x1.gun == x2.gun){
                return 0;                  // Fonksiyonun 0 döndürmesi x1 ve x2-nin eşit olduğunu gösteriyor.
            }
        }
    }    
}

int para_hesabi(struct tarih x1, struct tarih x2, int fiyat){
    int cevap=0;
    if (x1.yil == x2.yil){
        if(x1.ay == x2.ay){
            cevap = (x2.gun - x1.gun) * fiyat;          // Eğer yıl ve ay eşitse günlerin farkının oda fiyatı ile çarpımından ücreti buluruz.
        }else {                                                                             // Ama aynı ayda değilse o zaman ayları 31'den olan ve olmayan olarak ayırıyoruz. Hesabı ise 
                                                                                            // başlangıç ayın gün sayından başlangıç günü çıkıyoruyoruz, (bitiş ayın önemi yok) bitiş günü ve arada kalan aylardakı
            if(x1.ay != 2 || x1.ay != 4 || x1.ay != 6 || x1.ay != 9 || x1.ay != 11){        // günleri toplayarak toplam günü buluyoruz.
                if(x1.ay == 2){                                                             // Olmayanların içinde Şubat ayına özel başlık açıyoruz çünki 29 ve 28 olma olaylarını ayırmalıyız.
                    if(x1.yil%4 == 0){                  
                        cevap += (29 - x1.gun);         // Eğer hesapta Şubat ayı varsa olduğu yıla bakarak 29 olup olmadığına bakıyoruz.
                    }else {                             
                        cevap += (28 - x1.gun);
                    }
                }else{
                    cevap += (30 - x1.gun);
                }
            }else{
                cevap += (31 - x1.gun);
            }

            for (int i=(x1.ay + 1);i<x2.ay;i++){
                if(x1.ay != 2 || x1.ay != 4 || x1.ay != 6 || x1.ay != 9 || x1.ay != 11){
                    if(x1.ay == 2){
                        if(x1.yil%4 == 0){
                            cevap += 29;
                        }else {
                        cevap += 28;
                        }
                    }else{
                        cevap += 30;
                    }
                }else{
                  cevap += 31 ;
                } 
            } 

            cevap += x2.gun;
            cevap *= fiyat;      
        }

    } else {                        // Eğer yılda farklıysa aylarda yaptığımız yöntemin aynısını burdada yapıyoruz.

        struct tarih x3;

        x3.gun = 31;
        x3.ay = 12;
        x3.yil = x1.yil;

        cevap += para_hesabi(x1, x3, fiyat);

        for (int j = x1.yil+1; j < x2.yil; j++){
            if(j%4 == 0)
                cevap += 366*fiyat;
            else
                cevap += 365*fiyat;
        }

        x3.gun = 1;                 
        x3.ay = 1;
        x3.yil = x2.yil;

        cevap += para_hesabi(x3, x2, fiyat);

        cevap += fiyat;     // Eğer sonda fiyatı birdaha eklemezsek yılbaşı gününü saymıyıcak.
    }

    
    return cevap;
}

void Menu(){
    
    toplam_bilgiler_al();       // Program main() fonksiyonunun ardından Menu()'yü çalıştırdığından toplam rezerv, iptal ve geliri en başta alıyoruz.
    
    int islem=0;

    while(islem==0){
        sil();                  // Terminali temizliyoruz.
        printf("Rezervasyon Ana Ekranı\n\nYapmak istediğiniz işlemi seçin:\n1-Bilgileri göster\n2-Rezervasyon yap\n3-Rezarvasyon sil\n4-Çıkış\n\nİşlem: ");
        scanf("%d", &islem);

        switch (islem)
        {
        case 1:
            sil();
            Bilgiler();
            break;

        case 2:
            sil();
            Rezervasyon_ekle();
            break;

        case 3:
            sil();
            Rezervasyon_kaldir();
            break;

        case 4:
            sil();
            islem = 5; // While-dan çıkmak için.
            break;

        default:
            sil();
            printf("Yanlış tuşladınız.\n");
            islem = 0; // Yeniden while-ı döndürmek için.
            break;  

        }
    }
} 

void toplam_bilgiler_al(){              // gelir.txt metin belgesinde tutulan toplam rezerv, iptal sayı ve geliri almak için kullanıyoruz
    
    FILE *ftoplam;

    ftoplam = fopen("gelir.txt", "r");

    fscanf(ftoplam, "%d, %d, %d", &toplam_rezervasyon, &toplam_iptal, &toplam_kazanc);

    fclose(ftoplam);
}

void sil(){                      // prinf kalıbı sayesinde terminali temizliyoruz.
    printf("\e[1;1H\e[2J");
}

void Bilgiler(){
    struct oda mekan[30];                           // Rezervasyon tarihlerini ve s. bilgileri almak için oluşturduk.
    char *oda_isim;                                 // Yukarıda yazdığım üzere oda isimlerini "char" olarak değilde, işlem kolaylığı olsun 
    int en_buyuk=0;                                 // diye "int" olarak tutduk. Burda ise tabloda bastıracağımız için "char"-a dönüştürüyoruz.

    for (int i = 0; i < 30; i++)                    // Bilgileri yer() fonksiyonu vasıtası ile sonra işlemek üzere alıyoruz.
    {
        mekan[i] = yer(i);
    }
    

    for(int j=0; j<30; j++){                        // Tabloda gereksiz yerler kalmasın diye en fazla rezervasyon sayını bulup ona göre bir tablo oluşturucaz.
        if(mekan[j].rezervsayi > en_buyuk)
            en_buyuk = mekan[j].rezervsayi;  
    }

    printf("\nOda isim     |Numara   |Rezervasyon tarihi  |Rezarvasyon numarası |");    // Tablonun değişmez kısımlarını printf le bastırıyoruz.
    
    for(int j=1; j<en_buyuk; j++)                                                       // Bir odada birden fazla rezervasyon ola bileceğinden varsa eğer onlarıda göstermesi için.
        printf("Rezervasyon tarihi  |Rezarvasyon numarası |");

    printf("\n");
    printf("-------------|---------|--------------------|---------------------|");      // Yukardakiların aynısını çizgiler için dekullanıyoruz.

    for(int j=1; j<en_buyuk; j++){
        printf("--------------------|---------------------|");
    }
    printf("\n");
  
    for (int i =0; i<30 ;i++){
    
        switch (mekan[i].isim_num)                        // Odalara "isim_num"-a göre isimlerini atıyoruz.
        {
        case 1:
            oda_isim = "Küçük Oda    ";
            break;

        case 2:
            oda_isim = "Klasik Oda   ";
            break;

        case 3:
            oda_isim = "Geniş Oda    ";
            break;

        case 4:
            oda_isim = "Lüks Oda     ";
            break;

        case 5:
            oda_isim = "Kral Dairesi ";
            break;

        default:
            break;
        }                                                              
                                                            /** Tablodakı boşlukları aldığımız bilgilerle dolduruyoruz **/
                                                /** "\033[1;31m" gibi "\033" le başlayan yazılar renklendirme için kullanıyoruz  **/

        if (mekan[i].rezervsayi == 0)
            printf("%s|%d.%d      |\033[1;32m%2d.%2d.%2d - %2d.%2d.%2d \033[0m|\033[1;32mN%2d\033[0m                  |", oda_isim, mekan[i].isim_num, mekan[i].num, mekan[i].rezervbaslangic[0].gun, mekan[i].rezervbaslangic[0].ay, mekan[i].rezervbaslangic[0].yil, mekan[i].rezervbitis[0].gun, mekan[i].rezervbitis[0].ay, mekan[i].rezervbitis[0].yil, mekan[i].rezervbaslangic[0].rezerv_No);
        else 
            printf("%s|%d.%d      |\033[1;31m%2d.%2d.%2d - %2d.%2d.%2d \033[0m|\033[1;31mN%2d\033[0m                  |", oda_isim, mekan[i].isim_num, mekan[i].num, mekan[i].rezervbaslangic[0].gun, mekan[i].rezervbaslangic[0].ay, mekan[i].rezervbaslangic[0].yil, mekan[i].rezervbitis[0].gun, mekan[i].rezervbitis[0].ay, mekan[i].rezervbitis[0].yil, mekan[i].rezervbaslangic[0].rezerv_No);


        for (int j = 1; j < en_buyuk; j++){
            if(!mekan[i].rezervbaslangic[j].gun)
                printf("\033[1;32m 0. 0. 0 -  0. 0. 0 \033[0m|\033[1;32mN 0\033[0m                  |");
            else
                printf("\033[1;31m%2d.%2d.%2d - %2d.%2d.%2d \033[0m|\033[1;31mN%2d\033[0m                  |", mekan[i].rezervbaslangic[j].gun, mekan[i].rezervbaslangic[j].ay, mekan[i].rezervbaslangic[j].yil, mekan[i].rezervbitis[j].gun, mekan[i].rezervbitis[j].ay, mekan[i].rezervbitis[j].yil, mekan[i].rezervbaslangic[j].rezerv_No);
        }
        printf("\n");
    }
    printf("\n\n\nYapılan Toplam Rezervasyon Kaydı: %d.\nYapılan Toplam Rezervasyon İptali: %d.\nToplam Gelir: %d TL. \n\n", toplam_rezervasyon, toplam_iptal, toplam_kazanc);
}

void Rezervasyon_ekle(){
    
    FILE *foda , *fgelir;

    struct tarih baslangic, bitis, bu = bugun_tarih(bu), baslangic_tarih[6][10], bitis_tarih[6][10];
    struct oda mekan[30], yeni_rezervasyon;

    int hata, dongu=0, islem, sonra_olan_tarih_1, sonra_olan_tarih_2, kontrol, oda_num, ucret, line,en_buyuk=0;
    char *oda_isim, *durum[6][10], yanit = 'h', *son_durum[6];

    for (int i = 0; i < 30; i++)                    // Bilgileri yer() fonksiyonu vasıtası ile sonra işlemek üzere alıyoruz.
        mekan[i] = yer(i);
    
    for(int j=0; j<30; j++){                        // Tabloda gereksiz yerler kalmasın diye en fazla rezervasyon sayını bulup ona göre bir tablo oluşturucaz.
        if(mekan[j].rezervsayi > en_buyuk)
            en_buyuk = mekan[j].rezervsayi;  
    }

    baslangic.rezerv_No = toplam_rezervasyon + 1;   // Rezervasyon numarasını ayarlıyoruz.


    /** Rezervasyon yaptıracağımız tarihleri alıcaz **/
 
    printf("Rezervasyon yaptırmak istediğiniz tarihi girin: \n");
    while(dongu==0){                                    // Bu döngü kontrol amaçlı, eğer başlangıç tarih bitiş tarihten sonraysa tarihleri yeniden ısteyecek.
        hata = 1;
        while (hata==1){                                // Bu döngü başlangıç tarihinin doğruluğunu kontrol ediyor.

            printf("Başlangıç tarihi: ");
            scanf("%d.%d.%d", &baslangic.gun, &baslangic.ay, &baslangic.yil);

            if (baslangic.gun > 31 || baslangic.gun < 1){               
                printf("Hatalı tarih girdiniz.\n");
                hata = 1;
            }else if (baslangic.gun == 31){
                if (baslangic.ay == 2 || baslangic.ay == 4 || baslangic.ay == 6 || baslangic.ay == 9 || baslangic.ay == 11){
                    printf("Hatalı tarih girdiniz.\n"); 
                    hata = 1; 
                } else {
                    hata = 0;
                }
            }else if (baslangic.gun == 30){
                if (baslangic.ay == 2){
                    printf("Hatalı tarih girdiniz.\n");
                    hata = 1;
                } else {
                    hata = 0;
                }
            }else if (baslangic.gun == 29 && baslangic.ay == 2){  
                if (baslangic.yil%4 != 0){
                    printf("girdiğiniz yılda Şubat 28 gün.\n");
                    hata = 1;
                } else {
                    hata = 0;
                }
            }else if (baslangic.ay >12 || baslangic.ay < 1){
                printf("Hatalı tarih girdiniz.\n");
                hata =1;
            }else{
                hata = 0;
            }

        }

        hata = 1;
        while(hata==1){                                 // Bu döngü bitiş tarihinin doğruluğunu kontrol ediyor.

            printf("Bitiş tarihi: ");
            scanf("%d.%d.%d", &bitis.gun, &bitis.ay, &bitis.yil);

            if (bitis.gun > 31){
                printf("Hatalı tarih girdiniz.\n");
                hata = 1;
            }else if (bitis.gun == 31){
                if (bitis.ay == 2 || bitis.ay == 4 || bitis.ay == 6 || bitis.ay == 9 || bitis.ay == 11){
                    printf("Hatalı tarih girdiniz.\n");  
                    hata = 1;
                } else {
                    hata = 0;
                }
            }else if (bitis.gun == 30){
                if (bitis.ay == 2){
                    printf("Hatalı tarih girdiniz.\n");
                    hata = 1;
                } else {
                    hata = 0;
                }
            }else if (bitis.gun == 29 && bitis.ay == 2){ 
                if (bitis.yil%4 != 0){
                    printf("Hatalı tarih girdiniz.\n");
                    hata = 1;
                } else {
                    hata = 0;
                }
            }else if (bitis.ay >12 || bitis.ay < 1){
                printf("Hatalı tarih girdiniz.\n");
            } else {
                hata=0;
            }
        }

        sonra_olan_tarih_1 = karsilastirma(bitis, baslangic);           // Bitiş tarihi başlangıç tarihinden sonraysa 1, önceyse 2, aynı günse 0 döndürecek.
        sonra_olan_tarih_2 = karsilastirma(baslangic, bu);              // Baçlangıç tarihi bu günün tarihinden sonraysa 1, önceyse 2, aynı günse 0 döndürecek.

        if(sonra_olan_tarih_1 != 1 || sonra_olan_tarih_2 == 2){         // Yukardakı cevaplara göre son kontrolü yapıyoruz.
            printf("Hatalı tarih girdiniz \n");
            dongu = 0;
        }else 
            dongu = 1;
       
    }
    
    /** 
     * Başlangıç ve bitiş tarihlerini aldığımıza göre artık oda seçe biliriz.
     * Odalar değişmez olduğundan direk printf ile bastırıyoruz.
     * Sonrasında ise seçimini istiyoruz ve seçimine göre oda isimlerini atıyoruz.
     * **/
    sil();
    printf("N |Odalar       |Fiyat  |\n--|-------------|-------|\n1 |Küçük oda    |100 TL |\n2 |Klasik oda   |150 TL |\n3 |Geniş oda    |200 TL |\n4 |Lüks oda     |250 TL |\n5 |Kral Dairesi |300 TL |\n");
    printf("\n");

   
    while(dongu==1){                // Oda isim numarasının doğru girildiğine emin olmak için kullanıyoruz.

        printf("\nRezervasyon yaptırmak istediğiniz odayı seçin: ");
        scanf("%d", &islem);

        switch (islem)                          // Aşağıdaki case lerde girilen oda isim numarasına göre rezervasyon tarihlerini alıyoruz.
        {
        case 1:
            for(int i=0;i<6;i++){
                oda_isim = "Küçük Oda    ";
                baslangic_tarih[i][0] = mekan[i].rezervbaslangic[0];
                bitis_tarih[i][0] = mekan[i].rezervbaslangic[0];

                for(int j=1;j<mekan[i].rezervsayi;j++){
                    baslangic_tarih[i][j] = mekan[i].rezervbaslangic[j];
                    bitis_tarih[i][j] = mekan[i].rezervbaslangic[j];
                }
                dongu=0;
            }
            break;

        case 2:
            for(int i=6;i<12;i++){
                oda_isim = "Klasik Oda   ";
                baslangic_tarih[i-6][0] = mekan[i].rezervbaslangic[0];
                bitis_tarih[i-6][0] = mekan[i].rezervbaslangic[0];

                for(int j=1;j<mekan[i].rezervsayi;j++){
                    baslangic_tarih[i-6][j] = mekan[i].rezervbaslangic[j];
                    bitis_tarih[i-6][j] = mekan[i].rezervbaslangic[j];
                }
                dongu=0;
            }
            break;

        case 3:
            for(int i=12;i<18;i++){
                oda_isim = "Geniş Oda    ";
                baslangic_tarih[i-12][0] = mekan[i].rezervbaslangic[0];
                bitis_tarih[i-12][0] = mekan[i].rezervbaslangic[0];

                for(int j=1;j<mekan[i].rezervsayi;j++){
                    baslangic_tarih[i-12][j] = mekan[i].rezervbaslangic[j];
                    bitis_tarih[i-12][j] = mekan[i].rezervbaslangic[j];
                }
                dongu=0;
            }
            break;

        case 4:
            for(int i=18;i<24;i++){
                oda_isim = "Lüks Oda     ";
                baslangic_tarih[i-18][0] = mekan[i].rezervbaslangic[0];
                bitis_tarih[i-18][0] = mekan[i].rezervbaslangic[0];

                for(int j=1;j<mekan[i].rezervsayi;j++){
                    baslangic_tarih[i-18][j] = mekan[i].rezervbaslangic[j];
                    bitis_tarih[i-18][j] = mekan[i].rezervbaslangic[j];
                }
                dongu=0;
            }
            break;

        case 5:
            for(int i=24;i<30;i++){
                oda_isim = "Kral Dairesi ";
                baslangic_tarih[i-24][0] = mekan[i].rezervbaslangic[0];
                bitis_tarih[i-24][0] = mekan[i].rezervbaslangic[0];

                for(int j=1;j<mekan[i].rezervsayi;j++){
                    baslangic_tarih[i-24][j] = mekan[i].rezervbaslangic[j];
                    bitis_tarih[i-24][j] = mekan[i].rezervbaslangic[j];
                }
                dongu=0;
            }
            break;

        default:
            printf("Hatalı oda numarası girdiniz.");
            dongu=1;
            break;
        }
    } 


    // Aşağıda kullanıcıya hangi tarihte ve hangi odayı seçtiyi gösteriyoruz ve tablonun üst kısmını yapıyoruz.

    sil();

    printf("%d.%d.%d - %d.%d.%d tarihleri arasında %d numaralı odalar hakkında bilgiler:\n\n", baslangic.gun, baslangic.ay, baslangic.yil, bitis.gun, bitis.ay, bitis.yil, islem);

    printf("\nOda isim     |Numara   |");

    printf("Durum |\n");

    printf("-------------|---------|");

    printf("------|\n");

    for (int i = 0; i < 6; i++){                // Bu döngüde oda durumunu kontrol ediyoruz. "i" oda numarasını gösteriyor.
        for (int j = 0; j < en_buyuk; j++){     // Burada ise "j" rezervasyonu göstermekte.

            int yoklama1 = karsilastirma(baslangic, baslangic_tarih[i][j]);         // Tarihleri karşılaştırıyoruz.
            int yoklama2 = karsilastirma(bitis, bitis_tarih[i][j]);
            int yoklama3 = karsilastirma(baslangic, bitis_tarih[i][j]);
            int yoklama4 = karsilastirma(bitis, baslangic_tarih[i][j]);

            son_durum[i] = "Boş ";    // Durumu default olarak Boş yapıyoruz.

            if((yoklama2 == 1 && yoklama3 ==1) || (yoklama1 == 2 && yoklama4 == 2) || (baslangic_tarih[i][j].gun == 0)){  // Kontrolü yapıyoruz.
                durum[i][j] = "Boş ";
            } else{                             // Eğer girilen tarih odadaki her hangi bir tarihle çakışıyorsa, durum Dolu oluyor ve "j" döngüsünden çıkıyor.
                durum[i][j] = "Dolu";
                son_durum[i] = "Dolu";
                j = en_buyuk;
            }      
        }

        // Eğer oda doluysa kırmızı, boşsa yeşil bastıracak.

        if (son_durum[i] == "Dolu")
            printf("\033[1;31m%s\033[0m|\033[1;31m%d.%d\033[0m      |\033[1;31m%s\033[0m  |\n",oda_isim, islem, i+1, son_durum[i]);
        else 
            printf("\033[1;32m%s\033[0m|\033[1;32m%d.%d\033[0m      |\033[1;32m%s\033[0m  |\n",oda_isim, islem, i+1, son_durum[i]);

    }

    while(dongu==0){                // Oda numarası seçiliyor ve kontrol ediliyor.

        printf("\nKalmak istediğiniz odayı seçin: ");
        scanf("%d.%d", &kontrol, &oda_num);
        
        if(kontrol!=islem || oda_num>6 || oda_num<1 ){
            printf("Hatalı oda numarası girdiniz.\n");    
            dongu=0;
        }else if(son_durum[oda_num-1] == "Dolu"){
            printf("Dolu oda numarası girdiniz.\n");
            dongu=0;
        }else{
            dongu = 1;
        }
        
    }

    sil();

    ucret = para_hesabi(baslangic, bitis, (islem+1)*50);    // Oda ücretini hesaplıyoruz.

    if(son_durum[kontrol] = "Boş "){                         // Eğer oda boşsa son bir kez daha onay alıyoruz.
        while(dongu == 1){
            sil();

            printf("\n%d.%d numaralı odayı rezerve etmek üzresiniz.\nOdanızın toplam ücreti \033[3m%d\033[0m TL.\n\nOnaylıyormusunuz? (\033[1;32mE\033[0m/\033[1;31mH\033[0m) \nYanıt: ", islem, oda_num, ucret);
            scanf("%s", &yanit);

            switch (yanit)
            {
            case 'E':
                printf("\nRezervasyonunuz Yapıldı!\nRezervasyon numaranız - \033[1;38mN%2d\n\033[0m", baslangic.rezerv_No);
                dongu = 0;
                break;

            case 'H':
                printf("\nRezervasyonunuz Onaylamadınız.\n");
                dongu = 0;
                break;

            case 'e':
                printf("\nRezervasyonunuz Yapıldı!\nRezervasyon numaranız - \033[1;38mN%2d\n\033[0m", baslangic.rezerv_No);
                dongu = 0;
                break;

            case 'h':
                printf("\nRezervasyonunuz Onaylamadınız.\n");
                dongu = 0;
                break;

            default:
                printf("\nHatalı yanıt girdiniz.\n");
                dongu = 1;
                break;
            }
        }  
    }

    if (yanit == 'e' || yanit == 'E')               // Eğer kullanıcı rezervasyonu onayladıysa yeni razervasyonla birlikte bilgileri yeniden dosyaya kayıt ediyoruz.
    {                  
        int a[30];
        line = 6*(kontrol-1)+oda_num;
        
        foda = fopen("oda.txt", "w");               // fopen le dosyaları "w" modunda açıyoruz.
        fgelir = fopen("gelir.txt", "w");
    
        for (int i = 0; i<30; i++)                  // 30 oda olduğu için 30 satır bilgi olacak.
        {
            if(i == line-1)                         // Eğer rezervasyon eklediğimiz satırdaysak yeni bilgileri bastırıyoruz, değilsek eskileri.
            {
                fprintf(foda, "%d.%d - %d - %2d.%2d.%d - %2d.%2d.%2d (%2d)", kontrol, oda_num, mekan[line-1].rezervsayi+1, baslangic.gun, baslangic.ay, baslangic.yil, bitis.gun, bitis.ay, bitis.yil, baslangic.rezerv_No);
                for (int j = 0; j < mekan[i].rezervsayi; j++)
                    fprintf(foda, " | %2d.%2d.%2d - %2d.%2d.%2d (%2d)", mekan[i].rezervbaslangic[j].gun, mekan[i].rezervbaslangic[j].ay, mekan[i].rezervbaslangic[j].yil, mekan[i].rezervbitis[j].gun, mekan[i].rezervbitis[j].ay, mekan[i].rezervbitis[j].yil, mekan[i].rezervbaslangic[j].rezerv_No);
            }
            else
            {
                fprintf(foda, "%d.%d - %d - %2d.%2d.%2d - %2d.%2d.%2d (%2d)", mekan[i].isim_num, mekan[i].num, mekan[i].rezervsayi, mekan[i].rezervbaslangic[0].gun, mekan[i].rezervbaslangic[0].ay, mekan[i].rezervbaslangic[0].yil, mekan[i].rezervbitis[0].gun, mekan[i].rezervbitis[0].ay, mekan[i].rezervbitis[0].yil, mekan[i].rezervbaslangic[0].rezerv_No);
                for (int j = 1; j < mekan[i].rezervsayi; j++)
                    fprintf(foda, " | %2d.%2d.%2d - %2d.%2d.%2d (%2d)", mekan[i].rezervbaslangic[j].gun, mekan[i].rezervbaslangic[j].ay, mekan[i].rezervbaslangic[j].yil, mekan[i].rezervbitis[j].gun, mekan[i].rezervbitis[j].ay, mekan[i].rezervbitis[j].yil, mekan[i].rezervbaslangic[j].rezerv_No);
            }

            fprintf(foda, "\n");
        }

        toplam_kazanc += ucret;                                                                         // Son olarak toplam kazanca bu odanın ücretinide ekliyor ve dosyaya kayıt ederek rezervasyonu yapıyoruz.
        fprintf(fgelir, "%2d, %2d, %2d", toplam_rezervasyon+1, toplam_iptal, toplam_kazanc);

        fclose(foda);
        fclose(fgelir);
    }
}

void Rezervasyon_kaldir(){

    sil();

    FILE *foda, *fgelir;

    struct oda mekan[30];
    struct tarih baslangic_tarih, bitis_tarih, bu;

    int silinecek_rezerv, en_buyuk=0, ucret, yoklama , yanit=2, durum = 0, yoklama1 = 30;
    char cevap;

    bu = bugun_tarih(bu);


    for (int i = 0; i < 30; i++)                        // Bilgileri yer() fonksiyonu vasıtası ile sonra işlemek üzere alıyoruz.
        mekan[i] = yer(i);

    for (int j = 0; j < 30; j++){                       // Tabloda gereksiz yerler kalmasın diye en fazla rezervasyon sayını bulup ona göre bir tablo oluşturucaz.
        if(mekan[j].rezervsayi > en_buyuk)
            en_buyuk = mekan[j].rezervsayi;  
    }

    
    while(durum == 0)                                       // Kontrol için kullanıcaz.
    {
        printf("Rezervasyon Numaranızı Girin: ");           // Silinecek rezervasyon numarasını alıyoruz.
        scanf("%d", &silinecek_rezerv);

        for (int i = 0; i<30; i++)                                              // Bütün odaların,
        {
            for (int m = 0; m<en_buyuk; m++)                                    // bütün rezervasyonlarının numaralarını silinmesi ıstenen rezervasyon sayıyla karşılaştırıyoruz.
            {
                if (mekan[i].rezervbaslangic[m].rezerv_No == silinecek_rezerv)  // Eğer eşleşirse o rezervasyonları kullanmak üzere alıyoruz.
                {
                    baslangic_tarih = mekan[i].rezervbaslangic[m];
                    bitis_tarih = mekan[i].rezervbitis[m];
                    durum = 1;
                    m = en_buyuk;
                    i = 30;
                } 
                else                                                            // Hiç biri eşleşmezse demek ki, yanlış numara girilmiş.
                {
                    durum = 0;
                }
            }    
        }

        bu.gun ++;                                                              // Rezervasyonun son gün iptal edilmesi yasak olduğundan bu günün tarihiyle 
        yoklama = karsilastirma(baslangic_tarih, bu);                           // karşılaştırmamız fonksiyonun işlevinden dolayı "bu.gun"-ü 1 arttırıp fonksiyonu çağırıyoruz.

        if(durum == 0)                                                          // Eğer girilen rezervasyon yoksa programın başına gelicek.
        {
            printf("\nBöyle bir rezervasyon yok!\n\nYeniden deneyin\n\n");
        }
        else if(yoklama != 1)                                                   // Eğer rezervasyon tarihinden bi gün önce iptal edilmek isterse buraya gelecek.
        {
            while(yanit == 2)
            {
                printf("Rezervasyon iptali rezervasyon başlama tarihinden 1 gün öncesine kadar yapıla bilir.\n\nAna menüye dönmek için: 1\nBaşka rezervasyon iptali için: 0  girin.\n\nYanıtınız: ");
                scanf("%d", &yanit);

                switch (yanit)
                {
                case 1:
                    Menu();
                    break;

                case 0:
                    Rezervasyon_kaldir();
                    break;

                default:
                    printf("Yanlış yanıt girdiniz, lütfen geçerli bir yanıt girin.\n");
                    yanit = 2;
                    break;
                }
            }
        }
        else
        {
            printf("\033[1;38mN%2d\033[0m numaralı rezervasyonunuzu iptal etmek üzeresiniz.\n\nOnaylıyormusunuz? (\033[1;32mE\033[0m/\033[1;31mH\033[0m) \nYanıt: ", silinecek_rezerv);
            scanf("%s", &cevap);

            switch (cevap){
            
            case 'E':
                printf("\n\033[1;38mN%2d\033[0m numaralı rezervasyonunuzu başarıyla iptal ettiniz.\n", baslangic_tarih.rezerv_No);
                durum = 1;
                break;
            case 'H':
                while(yanit == 2){
                    printf("\nİşlemi Onaylamadınız.\n\n\nAna menüye dönmek için: 1\nBaşka rezervasyon iptali için: 0  girin.\n\nYanıtınız: ");
                    scanf("%d", &yanit);
                    switch (yanit)
                    {
                    case 1:
                        Menu();
                        break;
                    case 0:
                        Rezervasyon_kaldir();
                        break;
                    default:
                        printf("Yanlış yanıt girdiniz, lütfen geçerli bir yanıt girin.\n");
                        yanit = 2;
                        break;
                    }
                }
            case 'e':
                printf("\n\033[1;38mN%2d\033[0m numaralı rezervasyonunuzu başarıyla iptal etdiniz.\n", baslangic_tarih.rezerv_No);
                durum = 1;
                break;
            case 'h':
                while(yanit == 2){
                    printf("\nİşlemi Onaylamadınız.\n\n\nAna menüye dönmek için: 1\nBaşka rezervasyon iptali için: 0  girin.\n\nYanıtınız: ");
                    scanf("%d", &yanit);
                    switch (yanit)
                    {
                    case 1:
                        Menu();
                        break;
                    case 0:
                        Rezervasyon_kaldir();
                        break;
                    default:
                        printf("Yanlış yanıt girdiniz, lütfen geçerli bir yanıt girin.\n");
                        yanit = 2;
                        break;
                    }
                }
            default:
                printf("\nHatalı yanıt girdiniz.\n");
                durum = 0;
                break;
            }
        }  
    }
    
    foda = fopen("oda.txt", "w");           // fopen le dosyaları "w" modunda açıyoruz.
    fgelir = fopen("gelir.txt", "w");

    for (int i = 0; i<30; i++)                                                                              // Bütün odaları ve 
    {
        for (int m = 0; m<en_buyuk; m++)                                                                    // odaların rezervasyonlarını tarıyoruz.
        {
            if(mekan[i].rezervbaslangic[m].rezerv_No == baslangic_tarih.rezerv_No)                          // Rezervasyonunu silmek istediğimiz odaya geldiğimizde buraya giriyoruz.
            {
                ucret = para_hesabi(baslangic_tarih, bitis_tarih, (mekan[i].isim_num+1)*50);                // Iptalin ardından toplam paradan ne kadar çıkıcağımızı hesaplıyoruz.

                if (mekan[i].rezervsayi == 1)                                                               // Eğer rezerv sayı 1 tane ise direk 0'ları bastırıyoruz :D
                {
                    fprintf(foda, "%d.%d - 0 -  0. 0. 0 -  0. 0. 0 ( 0)", mekan[i].isim_num, mekan[i].num);
                } 
                else                                                                                        // Boş değilse 
                {
                    mekan[i].rezervsayi --;                                                                 // Önce rezerv sayından 1 düşüyoruz.

                    for (int j = m; j < mekan[i].rezervsayi; j++)                                           // Sonra "j"-i "m"-e eşitleyip (yani silinecek rezervasyona) eğer varsa 
                    {                                                                                       // ondan sonraki rezervasyonları bi geriye çekiyoruz.
                        mekan[i].rezervbaslangic[j] = mekan[i].rezervbaslangic[j+1];
                        mekan[i].rezervbitis[j] = mekan[i].rezervbitis[j+1];
                    }
                                                                                                            // Sonra da dosyaya bastırıyoruz.
                    
                    fprintf(foda, "%d.%d - %d - %2d.%2d.%2d - %2d.%2d.%2d (%2d)", mekan[i].isim_num, mekan[i].num, mekan[i].rezervsayi, mekan[i].rezervbaslangic[0].gun, mekan[i].rezervbaslangic[0].ay, mekan[i].rezervbaslangic[0].yil, mekan[i].rezervbitis[0].gun, mekan[i].rezervbitis[0].ay, mekan[i].rezervbitis[0].yil, mekan[i].rezervbaslangic[0].rezerv_No);
        
                    for (int j = 1; j < mekan[i].rezervsayi; j++)
                        fprintf(foda, " | %2d.%2d.%2d - %2d.%2d.%2d (%2d)", mekan[i].rezervbaslangic[j].gun, mekan[i].rezervbaslangic[j].ay, mekan[i].rezervbaslangic[j].yil, mekan[i].rezervbitis[j].gun, mekan[i].rezervbitis[j].ay, mekan[i].rezervbitis[j].yil, mekan[i].rezervbaslangic[j].rezerv_No);   
                } 

                m = en_buyuk;                         // "m"-yi "en_buyuk"-e getiriyoruzki bu oda için döngü bir daha dönmesin.
            }
            else
            {
                if (m==en_buyuk-1)                    // Değişiklik yapılmayan satırları bastırıcaz.
                {
                    fprintf(foda, "%d.%d - %d - %2d.%2d.%2d - %2d.%2d.%2d (%2d)", mekan[i].isim_num, mekan[i].num, mekan[i].rezervsayi, mekan[i].rezervbaslangic[0].gun, mekan[i].rezervbaslangic[0].ay, mekan[i].rezervbaslangic[0].yil, mekan[i].rezervbitis[0].gun, mekan[i].rezervbitis[0].ay, mekan[i].rezervbitis[0].yil, mekan[i].rezervbaslangic[0].rezerv_No);
        
                    for (int j = 1; j < mekan[i].rezervsayi; j++)
                        fprintf(foda, " | %2d.%2d.%2d - %2d.%2d.%2d (%2d)", mekan[i].rezervbaslangic[j].gun, mekan[i].rezervbaslangic[j].ay, mekan[i].rezervbaslangic[j].yil, mekan[i].rezervbitis[j].gun, mekan[i].rezervbitis[j].ay, mekan[i].rezervbitis[j].yil, mekan[i].rezervbaslangic[j].rezerv_No);
                }
            }
        }
        fprintf(foda, "\n");
    }

    toplam_kazanc -= ucret;                                                                 // Son olarak toplam kazanca bu odanın ücretinide ekliyor ve dosyaya kayıt ederek rezervasyonu yapıyoruz.
    fprintf(fgelir, "%2d, %2d, %2d", toplam_rezervasyon, toplam_iptal+1, toplam_kazanc);

    fclose(foda);
    fclose(fgelir);
    
}

void main()   // Başlıyoruz.
{
    setlocale(LC_ALL,"Turkish");
    Menu();    
}
