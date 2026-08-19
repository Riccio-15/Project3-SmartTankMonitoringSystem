#ifndef __CONTEXT__
#define __CONTEXT__

class Context {

  public:
    static void setLevel(int levelMm);
    static int getLevel();

    static void setNetworkOk(bool ok);
    static bool isNetworkOk();

    // true quando SonarTask ha scritto un nuovo campione non ancora pubblicato da task di mqqtt
    static bool hasNewLevelToPublish();
    static void setNewLevelToPublish(bool value);

    static Context &getInstance();

  private:
    Context();

  private:
    int level;
    bool networkOk;
    bool newLevelToPublish;
};

#endif