#include "render_config.h"

RenderConfig::RenderConfig() {
  _showValues = true;
  _showVectors = true;
  _showTraces = true;

  _traceLifetime = 120;
}

void RenderConfig::setShowValues(bool showValues) {
  _showValues = showValues;
}

bool RenderConfig::getShowValues() {
  return _showValues;
}

void RenderConfig::setShowVectors(bool showVectors) {
  _showVectors = showVectors;
}

bool RenderConfig::getShowVectors() {
  return _showVectors;
}

void RenderConfig::setShowTraces(bool showTraces) {
  _showTraces = showTraces;
}

bool RenderConfig::getShowTraces() {
  return _showTraces;
}

void RenderConfig::setTraceLifetime(int traceLifetime) {
  _traceLifetime = traceLifetime;
}

int RenderConfig::getTraceLifetime() {
  return _traceLifetime;
}

bool RenderConfig::_showValues = true;
bool RenderConfig::_showVectors = true;
bool RenderConfig::_showTraces = true;
int RenderConfig::_traceLifetime = 120;
