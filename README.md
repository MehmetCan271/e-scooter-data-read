# e-scooter-data-read
E-SCOOTER DATA READ PROJECT

#####This program is a software application that monitors and records data from various sensors of an electric scooter. The program utilizes hardware components such as the NEO 7-M GPS receiver, MPU6050 gyroscope sensor, and LTS25 current sensor to continuously gather data.

GPS data is obtained from the NEO 7-M GPS receiver to determine the scooter's position. This data enables tracking of where the scooter is being used and its movement trajectory.

Gyroscope data is collected from the MPU6050 gyroscope sensor to measure the scooter's tilt and orientation. This information assists in maintaining the scooter's balance and optimizing the user's riding experience. Additionally, a Kalman filter is applied to the gyroscope data for more accurate and smooth processing, resulting in precise determination of the scooter's position and a stable riding experience for the user.

Current values are acquired from the LTS25 current sensor to provide information about the scooter's battery status. This sensor measures the instantaneous voltage and current of the battery, offering real-time feedback to the user about the battery's condition.

The program combines all the collected data and records it onto an SD card. This allows users to monitor the scooter's performance, analyze usage patterns, and perform maintenance when necessary. Furthermore, it can be used to document any adverse incidents occurring during scooter usage for safety purposes.

By using the Arduino Mega microcontroller and various hardware components, this program provides a robust solution for monitoring the scooter and recording data. This enables users to optimize both the performance and safety of the scooter.#####

*************************************************************************************************************************************************************************************************************************************************************

#####Bu program, elektrikli scooter'ın çeşitli sensörlerinden gelen verileri izleyen ve kaydeden bir yazılım uygulamasıdır. Program, scooter üzerinde bulunan NEO 7-M GPS alıcısı, MPU6050 jiroskop sensörü ve LTS25 akım sensörü gibi çeşitli donanım bileşenlerini kullanarak sürekli olarak veri toplar. Mikrodenetleyici olarak Arduino Mega kullanılmıştır.

GPS verileri, scooter'ın konumunu belirlemek için NEO 7-M GPS alıcısından alınır. Bu veriler, kullanıcının scooterın nerede kullanıldığını ve hareket izini takip etmesini sağlar.

Jiroskop verileri, scooterın eğim ve yönelimini ölçmek için MPU6050 jiroskop sensöründen alınır. Bu veriler, scooterın dengesini korumasına ve kullanıcının sürüş deneyimini optimize etmesine yardımcı olur. Ayrıca, jiroskop verilerine bir Kalman filtresi uygulanır. Kalman filtresi, jiroskop verilerinin daha doğru ve pürüzsüz bir şekilde işlenmesini sağlar, böylece scooter'ın pozisyonunu daha hassas bir şekilde belirleyebilir ve kullanıcıya daha stabil bir sürüş deneyimi sunar.

Akım değerleri, scooterın batarya durumu hakkında bilgi sağlamak için LTS25 akım sensöründen alınır. Bu sensör, bataryanın anlık voltajını ve akımını ölçer, böylece kullanıcıya batarya durumu hakkında gerçek zamanlı bilgi sunar.

Program, toplanan tüm bu verileri birleştirir ve bir SD kartına kaydeder. Bu, kullanıcıların scooter'ın performansını izlemesini, kullanım örüntülerini analiz etmesini ve gerektiğinde bakım yapmasını sağlar. Ayrıca, güvenlik amaçları için, scooter'ın kullanımı sırasında oluşan herhangi bir olumsuz durumu belgelemek için de kullanılabilir.

Bu program, Arduino Mega mikrodenetleyicisi ve farklı donanım bileşenlerinden gelen verileri kullanarak scooterın izlenmesi ve verilerin kaydedilmesi için güçlü bir araç sağlar. Bu sayede kullanıcılar, hem scooterın performansını hem de güvenliğini optimize edebilirler.#####
