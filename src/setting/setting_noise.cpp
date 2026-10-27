// TODO : Rename setting_noise.cpp ?

#include "setting/setting.hpp"

// #include <vector> // TODO : ?

namespace 
{
    const char* noiseTypes[] = {"Value", "ValueFractal", "Perlin", "PerlinFractal", "Simplex", "SimplexFractal", "Cellular", "WhiteNoise", "Cubic", "CubicFractal"};
    const char* interps[] = {"Linear", "Hermite", "Quintic"};
    const char* fractalTypes[] = {"FBM", "Billow", "RigidMulti"};
    const char* cellularDistanceFunctions[] = {"Euclidean", "Manhattan", "Natural"};
    const char* cellularReturnTypes[] = {"CellValue", "NoiseLookup", "Distance", "Distance2", "Distance2Add", "Distance2Sub", "Distance2Mul", "Distance2Div"};
}

bool SettingWindow::OpenNoiseSettings(NoiseParameters& noiseParams, const std::string& labelSuffix, NoiseSettings& noiseSettings)
{
    bool needUpdate = false;
    if (ImGui::SliderFloat(("Frequency##"+labelSuffix).c_str(), &noiseParams.frequency, 0.5f, 10.f))
        needUpdate = true;
    if (ImGui::SliderInt(("Octaves##"+labelSuffix).c_str(), &noiseParams.octaves, 0, 8))
        needUpdate = true;
    if (ImGui::SliderInt(("Seed##"+labelSuffix).c_str(), &noiseParams.seed, 0, 255))
        needUpdate = true;
    
    if (ImGui::BeginCombo(("Noise Type##"+labelSuffix).c_str(), noiseTypes[noiseSettings.indexNoiseType])){ 
        for (unsigned int i = 0; i < IM_ARRAYSIZE(noiseTypes); i++){
            if (ImGui::Selectable(noiseTypes[i])) {
                noiseSettings.indexNoiseType = i;
                noiseParams.noiseType = static_cast<FastNoise::NoiseType>(noiseSettings.indexNoiseType);
                needUpdate = true;
            }
        }
        ImGui::EndCombo();
    }

    if (ImGui::BeginCombo(("Interp##"+labelSuffix).c_str(), interps[noiseSettings.indexInterp])){ 
        for (unsigned int i = 0; i < IM_ARRAYSIZE(interps); i++){
            if (ImGui::Selectable(interps[i])) {
                noiseSettings.indexInterp = i;
                noiseParams.interp = static_cast<FastNoise::Interp>(noiseSettings.indexInterp);
                needUpdate = true;
            }
        }
        ImGui::EndCombo();
    }

    if (ImGui::BeginCombo(("Fractal Type##"+labelSuffix).c_str(), fractalTypes[noiseSettings.indexFractalType])){ 
        for (unsigned int i = 0; i < IM_ARRAYSIZE(fractalTypes); i++){
            if (ImGui::Selectable(fractalTypes[i])) {
                noiseSettings.indexFractalType = i;
                noiseParams.fractalType = static_cast<FastNoise::FractalType>(noiseSettings.indexFractalType);
                needUpdate = true;
            }
        }
        ImGui::EndCombo();
    }

    if (ImGui::BeginCombo(("Cellular Distance Function##"+labelSuffix).c_str(), cellularDistanceFunctions[noiseSettings.indexCellularDistanceFunction])){ 
        for (unsigned int i = 0; i < IM_ARRAYSIZE(cellularDistanceFunctions); i++){
            if (ImGui::Selectable(cellularDistanceFunctions[i])) {
                noiseSettings.indexCellularDistanceFunction = i;
                noiseParams.cellularDistanceFunction = static_cast<FastNoise::CellularDistanceFunction>(noiseSettings.indexCellularDistanceFunction);
                needUpdate = true;
            }
        }
        ImGui::EndCombo();
    }

    if (ImGui::BeginCombo(("Cellular Return Type##"+labelSuffix).c_str(), cellularReturnTypes[noiseSettings.indexCellularReturnType])){ 
        for (unsigned int i = 0; i < IM_ARRAYSIZE(cellularReturnTypes); i++){
            if (ImGui::Selectable(cellularReturnTypes[i])) {
                noiseSettings.indexCellularReturnType = i;
                noiseParams.cellularReturnType = static_cast<FastNoise::CellularReturnType>(noiseSettings.indexCellularReturnType);
                needUpdate = true;
            }
        }
        ImGui::EndCombo();
    }

    return needUpdate;
}

bool SettingWindow::OpenSplinePlot(SplineData& spline)
{
    bool needUpdate = false;

    if (ImPlot::BeginPlot("Surface height spline")) {
        ImPlot::SetupAxis(ImAxis_X1, "Noise value", ImPlotAxisFlags_LockMin | ImPlotAxisFlags_LockMax);
        ImPlot::SetupAxis(ImAxis_Y1, "Height", ImPlotAxisFlags_LockMin | ImPlotAxisFlags_LockMax);
        ImPlot::SetupAxisLimits(ImAxis_X1, 0., 1.);
        ImPlot::SetupAxisLimits(ImAxis_Y1, 0., 1.);

        std::vector<float>& plotX = spline.plotX;
        std::vector<float>& plotY = spline.plotY;
        if (!plotX.empty() && !plotY.empty()) { // Should test plotX.size() != plotY.size() ?
            ImPlot::PlotScatter("Points", plotX.data(), plotY.data(), plotX.size());
            ImPlot::PlotLine("Points", plotX.data(), plotY.data(), plotX.size());
        }
        if (ImPlot::IsPlotHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
            ImPlotPoint mouse_pos = ImPlot::GetPlotMousePos();
            float x = mouse_pos.x;
            float y = mouse_pos.y;
            if (plotX.size() != 0){
                for (unsigned int i = 0 ; i < plotX.size() ; i++){
                    if (plotX[i] > x){
                        plotX.insert(plotX.begin()+i, x);
                        plotY.insert(plotY.begin()+i, y);
                        break;
                    }else if (i == plotX.size()-1){
                        plotX.push_back(x);
                        plotY.push_back(y);
                        break;
                    }
                }
            }else{
                plotX.push_back(x);
                plotY.push_back(y);
            }
            needUpdate = true;
        }

        ImGui::SetNextItemWidth(150.0f);
        if (ImGui::SliderFloat("First", &plotY.front(), 0.f, 1.f))
            needUpdate = true;
        ImGui::SameLine();
        ImGui::SetNextItemWidth(150.0f);
        if (ImGui::SliderFloat("Last", &plotY.back(), 0.f, 1.f))
            needUpdate = true;
        ImGui::SameLine();
        if (ImGui::Button("Clean")) {
            spline.Reset();
            needUpdate = true;
        }

        ImPlot::EndPlot();
    }
    
    return needUpdate;
}