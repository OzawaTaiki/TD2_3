#include "Bullistic.h"

#include <Framework/Camera/Camera.h>
#include <cassert>


void Bullistic::Initialize(const WorldTransform* _parent, const std::string& _name)
{
    assert(_parent != nullptr);

    InitJson(_name);

#ifdef _DEBUG
    ballistics_.reserve(MAX_MODEL_NUM);

    for (size_t i = 0; i < MAX_MODEL_NUM; ++i)
    {
        auto& model = ballistics_.emplace_back(std::make_unique<ObjectModel>());
        model->Initialize("Player/Ballistic.obj", "Ballistic");
        model->SetParent(_parent);
        model->GetMaterial(1)->enableLighting_ = false;
    }

#else
    ballistics_.reserve(modelNum_);

    for (size_t i = 0; i < modelNum_; ++i)
    {
        auto& model = ballistics_.emplace_back(std::make_unique<ObjectModel>());
        model->Initialize("Player/Ballistic.obj", "Ballistic");
        model->SetParent(_parent);
        model->GetMaterial(1)->enableLighting_ = false;
    }

#endif // _DEBUG

    for (size_t i = 0; i < modelNum_; ++i)
    {
        ballistics_[i]->scale_ = scale_;
        ballistics_[i]->translate_ = offset_;
        ballistics_[i]->translate_.z = (scale_.z + range_) * static_cast<float>(i);
        isDraw_[i] = true;
    }


}

void Bullistic::Update()
{
    ImGui();

    for (size_t i = 0; i < modelNum_; ++i)
    {
        ballistics_[i]->Update();
    }
}

void Bullistic::Draw(Camera* _camera)
{
    for (size_t i = 0; i < modelNum_; ++i)
    {
        ballistics_[i]->Draw(_camera, color_);
    }
}

void Bullistic::ImGui()
{
#ifdef _DEBUG

    bool isDirty = false;

    ImGui::Begin("Bullistic");

    ImGui::PushID(this);
    ImGui::Separator();

    if (ImGui::DragInt("ModelNum", reinterpret_cast<int*>(&modelNum_), 1, 0, MAX_MODEL_NUM))
        isDirty = true;
    if(ImGui::DragFloat3("Size", &scale_.x, 0.01f))
        isDirty = true;
    if (ImGui::DragFloat3("Offset", &offset_.x, 0.01f))
        isDirty = true;
    if (ImGui::DragFloat("Range", &range_, 0.01f))
        isDirty = true;

    if (isDirty)
    {
        InitBullistics();
    }

    ImGui::ColorEdit4("Color", &color_.x);

    if (ImGui::Button("Save"))
    {
        jsonBinder_->Save();
    }
    ImGui::Separator();
    ImGui::PopID();

    ImGui::End();

#endif // _DEBUG
}

void Bullistic::InitBullistics()
{
    isDraw_.fill(false);

    for (size_t i = 0; i < MAX_MODEL_NUM; ++i)
    {
        if (i < modelNum_)
        {
            ballistics_[i]->scale_ = scale_;
            ballistics_[i]->translate_ = offset_;
            ballistics_[i]->translate_.z += (scale_.z + range_) * static_cast<float>(i);

            isDraw_[i] = true;
        }
    }
}

void Bullistic::InitJson( const std::string& _name)
{
    jsonBinder_ = std::make_unique<JsonBinder>("Bullistic_" + _name, "Resources/Data/parameter/");

    jsonBinder_->RegisterVariable("ModelNum", reinterpret_cast<uint32_t*>(&modelNum_));
    jsonBinder_->RegisterVariable("Range", &range_);
    jsonBinder_->RegisterVariable("Size", &scale_);
    jsonBinder_->RegisterVariable("Offset", &offset_);
    jsonBinder_->RegisterVariable("Color", &color_);

}
