#include <jni.h>

extern "C" JNIEXPORT void JNICALL
Java_com_kmspofol_lib_Native_loadGame(JNIEnv* env, jobject obj,
                                    jobject ipkFilePath, jobject ioPath);

extern "C" JNIEXPORT void JNICALL
Java_com_kmspofol_lib_Native_freeGame(JNIEnv* env, jobject obj);
extern "C" JNIEXPORT void JNICALL
Java_com_kmspofol_lib_Native_drawGame(JNIEnv* env, jobject obj);
extern "C" JNIEXPORT void JNICALL
Java_com_kmspofol_lib_Native_keyGame(JNIEnv* env, jobject obj,
                                      jint state, jfloat x, jfloat y);

extern "C" JNIEXPORT void JNICALL
Java_com_kmspofol_lib_Native_multiKeyGame(JNIEnv* env, jobject obj,
                                      jint state, jint num, jfloatArray mx, jfloatArray my);

extern "C" JNIEXPORT void JNICALL
Java_com_kmspofol_lib_Native_resizeGame(JNIEnv* env, jobject obj,
                                      jint width, jint height);


extern "C" JNIEXPORT void JNICALL
Java_com_kmspofol_lib_Native_pauseGame(JNIEnv* env, jobject obj,
                                         jboolean pause);

extern "C" JNIEXPORT void JNICALL
Java_com_kmspofol_lib_Native_backKeyDown(JNIEnv* env, jobject obj);

extern "C" JNIEXPORT void JNICALL
Java_com_kmspofol_lib_Native_homeButton(JNIEnv* env, jobject obj);

void resizeApp(int width, int height);
void mainLoop();