#ifndef RENDER_CONFIG_H
#define RENDER_CONFIG_H


class RenderConfig {
    public:
      
      RenderConfig();

      static void setShowValues(bool);
      bool getShowValues();

      static void setShowVectors(bool);
      bool getShowVectors();

      static void setShowTraces(bool);
      bool getShowTraces();

      static void setTraceLifetime(int);
      int getTraceLifetime();

    private:
      static bool _showValues;
      static bool _showVectors;
      static bool _showTraces;

      static int _traceLifetime;

};


#endif // RENDER_CONFIG_H
