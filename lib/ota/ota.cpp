// #include "ota.h"

// esp32FOTA FOTA(HARDWARE_VERSION, FIRMWARE_VERSION, VALIDATE_FIRMWARE_SIGNATURE);

// // for github https
// const char *root_ca = R"ROOT_CA(
// -----BEGIN CERTIFICATE-----
// MIIEvjCCA6agAwIBAgIQBtjZBNVYQ0b2ii+nVCJ+xDANBgkqhkiG9w0BAQsFADBh
// MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3
// d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBD
// QTAeFw0yMTA0MTQwMDAwMDBaFw0zMTA0MTMyMzU5NTlaME8xCzAJBgNVBAYTAlVT
// MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxKTAnBgNVBAMTIERpZ2lDZXJ0IFRMUyBS
// U0EgU0hBMjU2IDIwMjAgQ0ExMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKC
// AQEAwUuzZUdwvN1PWNvsnO3DZuUfMRNUrUpmRh8sCuxkB+Uu3Ny5CiDt3+PE0J6a
// qXodgojlEVbbHp9YwlHnLDQNLtKS4VbL8Xlfs7uHyiUDe5pSQWYQYE9XE0nw6Ddn
// g9/n00tnTCJRpt8OmRDtV1F0JuJ9x8piLhMbfyOIJVNvwTRYAIuE//i+p1hJInuW
// raKImxW8oHzf6VGo1bDtN+I2tIJLYrVJmuzHZ9bjPvXj1hJeRPG/cUJ9WIQDgLGB
// Afr5yjK7tI4nhyfFK3TUqNaX3sNk+crOU6JWvHgXjkkDKa77SU+kFbnO8lwZV21r
// eacroicgE7XQPUDTITAHk+qZ9QIDAQABo4IBgjCCAX4wEgYDVR0TAQH/BAgwBgEB
// /wIBADAdBgNVHQ4EFgQUt2ui6qiqhIx56rTaD5iyxZV2ufQwHwYDVR0jBBgwFoAU
// A95QNVbRTLtm8KPiGxvDl7I90VUwDgYDVR0PAQH/BAQDAgGGMB0GA1UdJQQWMBQG
// CCsGAQUFBwMBBggrBgEFBQcDAjB2BggrBgEFBQcBAQRqMGgwJAYIKwYBBQUHMAGG
// GGh0dHA6Ly9vY3NwLmRpZ2ljZXJ0LmNvbTBABggrBgEFBQcwAoY0aHR0cDovL2Nh
// Y2VydHMuZGlnaWNlcnQuY29tL0RpZ2lDZXJ0R2xvYmFsUm9vdENBLmNydDBCBgNV
// HR8EOzA5MDegNaAzhjFodHRwOi8vY3JsMy5kaWdpY2VydC5jb20vRGlnaUNlcnRH
// bG9iYWxSb290Q0EuY3JsMD0GA1UdIAQ2MDQwCwYJYIZIAYb9bAIBMAcGBWeBDAEB
// MAgGBmeBDAECATAIBgZngQwBAgIwCAYGZ4EMAQIDMA0GCSqGSIb3DQEBCwUAA4IB
// AQCAMs5eC91uWg0Kr+HWhMvAjvqFcO3aXbMM9yt1QP6FCvrzMXi3cEsaiVi6gL3z
// ax3pfs8LulicWdSQ0/1s/dCYbbdxglvPbQtaCdB73sRD2Cqk3p5BJl+7j5nL3a7h
// qG+fh/50tx8bIKuxT8b1Z11dmzzp/2n3YWzW2fP9NsarA4h20ksudYbj/NhVfSbC
// EXffPgK2fPOre3qGNm+499iTcc+G33Mw+nur7SpZyEKEOxEXGlLzyQ4UfaJbcme6
// ce1XR2bFuAJKZTRei9AqPCCcUZlM51Ke92sRKw2Sfh3oius2FkOH6ipjv3U/697E
// A7sKPPcw7+uvTPyLNhBzPvOk
// -----END CERTIFICATE-----
// )ROOT_CA";

// CryptoMemAsset *MyRootCA = new CryptoMemAsset("Root CA", root_ca, strlen(root_ca) + 1);
// CryptoMemAsset *MyPubKey = new CryptoMemAsset("RSA Key", public_key, strlen(public_key) + 1);

// bool doOTAUpdate = false;
// unsigned long ota_last_message = 0;

// void ota_setup()
// {
//     FOTA.setManifestURL("https://raw.githubusercontent.com/hoeken/yarrboard/main/firmware/firmware.json");
//     FOTA.setRootCA(MyRootCA);
//     FOTA.setPubKey(MyPubKey);

//     FOTA.setUpdateBeginFailCb([](int partition)
//                               { Serial.printf("[ota] Update could not begin with %s partition\n", partition == U_SPIFFS ? "spiffs" : "firmware"); });

//     // usage with lambda function:
//     FOTA.setProgressCb([](size_t progress, size_t size)
//                        {
//       if( progress == size || progress == 0 )
//         Serial.println();
//       Serial.print(".");

//       //let the clients know every second and at the end
//       if (millis() - ota_last_message > 1000 || progress == size)
//       {
//         float percent = (float)progress / (float)size * 100.0;
//         sendOTAProgressUpdate(percent);
//         ota_last_message = millis();
//       } });

//     FOTA.setUpdateEndCb([](int partition)
//                         {
//     Serial.printf("[ota] Update ended with %s partition\n", partition==U_SPIFFS ? "spiffs" : "firmware" );
//       sendOTAProgressFinished(); });

//     FOTA.setUpdateCheckFailCb([](int partition, int error_code)
//                               {
//                                   Serial.printf("[ota] Update could not validate %s partition (error %d)\n", partition == U_SPIFFS ? "spiffs" : "firmware", error_code);
//                                   // error codes:
//                                   //  -1 : partition not found
//                                   //  -2 : validation (signature check) failed
//                               });

//     FOTA.printConfig();
// }

// void ota_loop()
// {
//     if (doOTAUpdate)
//     {
//         FOTA.handle();
//         doOTAUpdate = false;
//     }
// }