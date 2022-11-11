# Лабораторная работа №1
___
## Объяснение
___

1) Генерация ключа и iv
```c++
    void KeyGen(SecByteBlock& key, SecByteBlock& iv)
```
2) Шифрование используя чача
```c++
    void Enc(SecByteBlock& key, SecByteBlock& iv, std::string& plaintext, std::string& ciphertext)
```
3) Дешифрование используя чача.
```c++
    void Dec(SecByteBlock& key, SecByteBlock& iv, std::string& ciphertext, std::string& decrypted)
```
___
## Результат
___
![img.png](img.png)