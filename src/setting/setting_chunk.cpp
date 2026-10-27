#include "setting/setting.hpp"

bool SettingWindow::OpenSettings(DataFlatChunk& data)
{
    return false;
}

bool SettingWindow::OpenSettings(DataFullChunk& data)
{
    return false;
}

bool SettingWindow::OpenSettings(DataWaveChunk& data)
{
    if (ImGui::SliderFloat("Frequency##Wave", &data.frequency, 0.5f, 10.f))
        return true;
    return false;
}

bool SettingWindow::OpenSettings(DataEditorChunk& data)
{
    return false;
}

bool SettingWindow::OpenSettings(DataHeightmapChunk& data)
{
    bool needUpdate = false;

    needUpdate |= OpenNoiseSettings(data.noiseParams, "Heightmap", m_heightmapNoiseSettings);

    if (ImGui::Checkbox("Use spline##Heightmap", &data.spline.use))
        needUpdate = true;

    if (data.spline.use)
        needUpdate |= OpenSplinePlot(data.spline);
    
    return needUpdate;
}

bool SettingWindow::OpenSettings(DataCheeseChunk& data)
{
    bool needUpdate = false;

    if (ImGui::SliderFloat("Threshold##Cheese", &data.threshold, -1.f, 1.f))
        needUpdate = true;

    needUpdate |= OpenNoiseSettings(data.noiseParams, "Cheese", m_cheeseNoiseSettings);

    return needUpdate;
}

bool SettingWindow::OpenSettings(DataCaveChunk& data)
{
    bool needUpdate = false;

    if (ImGui::SliderInt("Iteration##Cave", &data.nrIteration, 0, 10))
        needUpdate = true;
    if (ImGui::SliderInt("Threshold##Cave", &data.threshold, 0, 8))
        needUpdate = true;

    if (ImGui::Checkbox("Use spline##Cave", &data.spline.use))
        needUpdate = true;
    
    if (data.spline.use)
        needUpdate |= OpenSplinePlot(data.spline);

    return needUpdate;
}