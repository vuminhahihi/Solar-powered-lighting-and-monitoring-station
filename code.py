import cv2

# Địa chỉ IP của ESP32-CAM (thay bằng IP thực tế của bạn)
url = "http://192.168.1.123/stream"

# Khởi tạo luồng video từ ESP32-CAM
cap = cv2.VideoCapture(url)

# Khởi tạo bộ phát hiện người sử dụng HOG + SVM có sẵn trong OpenCV
hog = cv2.HOGDescriptor()
hog.setSVMDetector(cv2.HOGDescriptor_getDefaultPeopleDetector())

while True:
    # Đọc khung hình từ luồng video
    ret, frame = cap.read()
    if not ret:
        print("Không đọc được khung hình")
        break

    # Phát hiện người trong khung hình
    boxes, weights = hog.detectMultiScale(frame, winStride=(8, 8))

    # Vẽ khung và ghi nhãn "Person" cho mỗi đối tượng phát hiện được
    for (x, y, w, h) in boxes:
        cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 255, 0), 2)
        cv2.putText(frame, "Person", (x, y - 10),
                    cv2.FONT_HERSHEY_SIMPLEX, 0.6, (0, 255, 0), 2)

    # Hiển thị khung hình đã xử lý
    cv2.imshow("ESP32-CAM Person Detection", frame)

    # Nhấn phím 'q' để thoát vòng lặp
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# Giải phóng tài nguyên
cap.release()
cv2.destroyAllWindows()
