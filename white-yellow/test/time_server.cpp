//#include <iostream>
//#include <exception>
//#include <string>
//using namespace std;
//
////string AskTimeServer() {
////    throw domain_error("asdf");
////    throw system_error(error_code());
////    return "12:03:33";
////}
//
//class TimeServer {
//public:
//    string GetCurrentTime() {
//        try {
//            LastFetchedTime = AskTimeServer();
//        }
//        catch (system_error&) {}
//
//        return LastFetchedTime;
//    }
//
//private:
//    string LastFetchedTime = "00:00:00";
//};
//
//int main() {
//    TimeServer ts;
//    try {
//        cout << ts.GetCurrentTime() << endl;
//    }
//    catch (exception& e) {
//        cout << "Exception got: " << e.what() << endl;
//    }
//    return 0;
//}
