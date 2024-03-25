#pragma once
#include "Primitivas.h"
#include <GL/glu.h>

class Camera
{
    private:
    vetor3 posicao; // Posição da câmera
    vetor3 direcao; // Direção para onde ela está olhando
    vetor3 up; // Topo da câmera
    vetor3 left; // vetor do movimento
    vetor3 velocidade;
    float escala;
    float yaw; // Ângulo de rotação da câmera;

    public:
    Camera(vetor3 posicao)
    {
        this->posicao = vetor3(posicao);
        this->direcao = vetor3(0.0, -1.0, 0.0);
        this->left = vetor3(-1.0, 0.0, 0.0);
        this->up = vetor3(0.0, 0.0, 1.0);
        this->velocidade = vetor3(0.0, 0.0, 0.0);
        this->escala = 0.25;
    }

    void ativar();

    void frente();
    void tras();
    void esquerda();
    void direita();
    void updateYaw(float);
    void update();

    private:
    
    float converterParaRadianos(float);
};

float Camera::converterParaRadianos(float angulo)
{
    return (angulo * M_PI) / 180.0;
}

void Camera::ativar()
{
    vetor3 look = this->posicao + this->direcao;

    gluLookAt(this->posicao.x, this->posicao.y, this->posicao.z, look.x, look.y, look.z, this->up.x, this->up.y, this->up.z);
}

void Camera::frente()
{
    this->velocidade = this->direcao * this->escala;
    this->posicao = this->posicao + this->velocidade;
}

void Camera::tras()
{
    this->velocidade = this->direcao * (-this->escala);
    this->posicao = this->posicao + this->velocidade;
}

void Camera::esquerda()
{
    this->velocidade = this->left * this->escala;
    this->posicao = this->posicao + this->velocidade;
}

void Camera:: direita()
{
    this->velocidade = this->left * (-this->escala);
    this->posicao = this->posicao + this->velocidade;
}

void Camera::updateYaw(float yaw)
{
    this->yaw += yaw;
}

void Camera::update()
{
    float angulo = this->converterParaRadianos(this->yaw);
    this->direcao.x = sin(angulo);
    this->direcao.y = cos(angulo);
    this->direcao.normaliza();
    this->left = this->up.produtoVetorial(this->direcao);
}
