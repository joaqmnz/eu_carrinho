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
    float carroYaw;

    public:
    Camera(vetor3 posicao)
    {
        this->posicao = vetor3(posicao);
        // float angulo = converterParaRadianos(35.0f); // Converta o ângulo para radianos
        this->direcao = vetor3(0.0f, -1.0, 0.0); 
        this->left = vetor3(1.0, 0.0, 0.0);
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
    void updateCarroYaw(float);
    void update();
    vetor3 getPosicao() const;
    float getYaw() const { return yaw; } 
    vetor3 getDirecao() const{   return direcao;}
    float getCarroYaw() const { return carroYaw; } 

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

void Camera::direita()
{
    this->updateYaw(0.4f); // Rotaciona a câmera para a direita
    this->updateCarroYaw(-0.4f); // Rotaciona o carro para a direita
}

void Camera::esquerda()
{
    this->updateYaw(-0.4f); // Rotaciona a câmera para a esquerda
    this->updateCarroYaw(+0.4f); // Rotaciona o carro para a esquerda
}


void Camera::updateYaw(float yaw)
{
    this->yaw += yaw;

    // Limita o ângulo de rotação entre 0 e 360 graus
    if (this->yaw >= 360.0f) {
        this->yaw -= 360.0f;
    } else if (this->yaw < 0.0f) {
        this->yaw += 360.0f;
    }

    this->update(); // Atualiza a direção da câmera de acordo com o novo ângulo de rotação
}

void Camera::update()
{
    float angulo = this->converterParaRadianos(this->yaw);
    this->direcao.x = sin(angulo);
    this->direcao.y = cos(angulo);
    this->direcao.normaliza();
    this->left = this->up.produtoVetorial(this->direcao);
}

void Camera::updateCarroYaw(float yaw)
{
    this->carroYaw += yaw;

    // Limita o ângulo de rotação entre 0 e 360 graus
    if (this->carroYaw >= 360.0f) {
        this->carroYaw -= 360.0f;
    } else if (this->carroYaw < 0.0f) {
        this->carroYaw += 360.0f;
    }
}


vetor3 Camera::getPosicao() const // Implementação do método getPosicao
{
    return posicao;
}
