#include <iNut.h>

iNut inut;// Khởi tạo đối tượng iNut

const int SO_LED = 2;
int led[SO_LED] = {8, 9}; //2 bóng led được nối với chân 8,9

int chanCamBienVatCan = 2;
boolean trangThaiVatCanTruoc = LOW;

void setup() {
  // Thiết lập chế độ OUTPUT cho 2 chân nối với led
  pinMode(led[0], OUTPUT);
  pinMode(led[1], OUTPUT);
  
  // Thiết lập chế độ INPUT cho chân nối với cảm biến
  pinMode(chanCamBienVatCan, INPUT);

  // Trạng thái ban đầu của các led là tắt
  digitalWrite(led[0], LOW);
  digitalWrite(led[1], LOW);

  // Lệnh thiết lập kết nối Arduino với iNut cảm biến,
  // trong đó 8 là số luồng dữ liệu của iNut cảm biến
  inut.setup(8);

  // Bắt buộc phải có bước này: Đặt giá trị ban đầu cho 8 luồng khi mới kết nối
  for (int i = 0; i < 8; i++) {
    // Lệnh đặt giá trị dữ liệu cho luồng thứ i
    // Ban đầu thì cứ cho cả 8 luồng nhận giá trị là 0
    inut.setValue(i, 0);
  }

  // Khai báo cho iNut cảm biến: Khi nhận được lệnh "ON" thì chạy hàm onLed
  inut.addCommand("ON", onLed);
  // Khai báo cho iNut cảm biến: Khi nhận được lệnh "OFF" thì chạy hàm offLed
  inut.addCommand("OFF", offLed);
}

void loop() {
  capNhatTrangThaiLed();
  capNhatTrangThaiAnhSang();
  kiemTraChongTrom();

  // Cần chạy câu lệnh này trong hàm loop
  inut.loop();
}

void onLed() {
  char *thamSo = inut.next();// Lấy tham số
  int giaTri = atoi(thamSo);// Lệnh atoi sẽ chuyển 1 chuỗi thành số nguyên
  int thuTuLed = giaTri - 216;// Theo bảng tra vị trí digital thì vị trí 0,1 luồng 1 là 216,217
  digitalWrite(led[thuTuLed], HIGH);
}

void offLed() {
  char *thamSo = inut.next();// Lấy tham số
  int giaTri = atoi(thamSo);// Lệnh atoi sẽ chuyển 1 chuỗi thành số nguyên
  int thuTuLed = giaTri - 216;// Theo bảng tra vị trí digital thì vị trí 0,1 luồng 1 là 216,217
  digitalWrite(led[thuTuLed], LOW);
}

void kiemTraChongTrom() {
  boolean trangThaiVatCan = digitalRead(chanCamBienVatCan);// Đọc giá trị cảm biến
  if (trangThaiVatCan != trangThaiVatCanTruoc) {
    if (trangThaiVatCan == HIGH) { // Nếu không có vật cản - Trạng thái cảm biến là HIGH
      // Lệnh đặt giá trị 1 (trạng thái HIGH) cho vị trí digital thứ 0 của luồng số 0
      inut.turnOn(0, 0);// turnOn(<luồng>, <vị trí digital>);
    } else { // Ngược lại nếu có vật cản
      // Lệnh đặt giá trị 0 (trạng thái LOW) cho vị trí digital thứ 0 của luồng số 0
      inut.turnOff(0, 0);// turnOff(<luồng>, <vị trí digital>);
      inut.alarm(0);// Gửi báo động đến máy chủ với mã báo động là 0
    }
    trangThaiVatCanTruoc = trangThaiVatCan;
  }
}

void capNhatTrangThaiLed() {
  // cập nhật dữ liệu cho các vị trí digital tương ứng với nút nhấn - Trạng thái các led
  for (int i = 0; i < SO_LED; i++) {
    /* Có thể dùng lệnh digitalRead để đọc trạng thái
       của một chân cho dù chân này ở trạng thái OUTPUT
    */
    boolean trangThaiLed = digitalRead(led[i]);
    if (trangThaiLed == HIGH) { // Nếu Led sáng
      // Lệnh đặt giá trị 1 (trạng thái HIGH) cho vị trí digital thứ i của luồng số 1
      inut.turnOn(1, i);// turnOn(<luồng>, <vị trí digital>);
    } else { // Nếu Led tắt
      // Lệnh đặt giá trị 0 (trạng thái LOW) cho vị trí digital thứ i của luồng số 1
      inut.turnOff(1, i);// turnOff(<luồng>, <vị trí digital>);
    }
  }
}

void capNhatTrangThaiAnhSang() {
  int val = analogRead(A5);// Đọc giá trị quang trở
  inut.setValue(2, val);// Đặt giá trị dữ liệu cho luồng số 2 là giá trị quang trở
}

