# Projection Matrix 유도

## 개요

**변환**: 다차원 공간을 그래픽에 표현하는 과정

| 종류 | 특징 |
|------|------|
| **원근 투영 (Perspective)** | 멀수록 이미지가 점점 작아지는 느낌 (원근감) |
| **직교 투영 (Orthogonal)** | 거리에 관계없이 크기가 일정 |

---

## 1. Perspective Projection Matrix

### 1.1 절두체 (Frustum) 구조

카메라는 $-z$ 방향을 바라본다.

```
        near       far
Camera   |          |
  *------+----------+------
        -n         -f
```

- Near plane: $z = -n$
- Far plane: $z = -f$
- 화면 크기: $width \times height$ (**near plane** 기준 = viewport)

### 1.2 닮음비 (Similar Triangles)

점 $P = (x, y, z)$를 near plane에 투영하면:

$$x_p = \frac{n \cdot x}{-z}, \quad y_p = \frac{n \cdot y}{-z}$$

### 1.3 NDC 정규화

$w$, $h$는 **near plane** 기준 크기 (viewport와 직접 대응).

투영된 좌표를 NDC $[-1, 1]$로 정규화:

$$x_{ndc} = \frac{x_p}{w/2} = \frac{2n}{width} \cdot \frac{x}{-z}$$

$$y_{ndc} = \frac{y_p}{h/2} = \frac{2n}{height} \cdot \frac{y}{-z}$$

동차 좌표계에서 $w_{clip} = -z$로 나누는 **perspective divide**를 활용:

$$x_{clip} = \frac{2n}{w} x, \quad y_{clip} = \frac{2n}{h} y, \quad w_{clip} = -z$$

### 1.4 Z 매핑 유도

$z_{clip} = az + b$로 정의. NDC z는 perspective divide 후:

$$z_{ndc} = \frac{az + b}{-z}$$

**Step 1 — 초기 경계 조건 (직관적 유도):**

Near가 앞쪽(+1), Far가 뒤쪽(-1)으로 설정:

$$f(n) = +1, \quad f(-f) = -1$$

연립방정식:

$$\begin{cases} -an + b = n \\[4pt] -af + b = -f \end{cases}$$

두 식을 빼면:

$$a(f - n) = f + n \implies a = \frac{f+n}{f-n}$$

대입하면:

$$b = \frac{2nf}{f-n}$$

**Step 2 — OpenGL Rasterizer 변환:**

OpenGL Rasterizer는 반대 convention을 사용:

$$\text{near} \rightarrow -1, \quad \text{far} \rightarrow +1$$

따라서 z에 $-1$을 곱해 부호를 뒤집으면:

$$\boxed{a = -\frac{f+n}{f-n}, \quad b = -\frac{2nf}{f-n}}$$

### 1.5 Perspective Projection Matrix

$$\begin{bmatrix} x' \\ y' \\ z' \\ w' \end{bmatrix} = \begin{bmatrix} \dfrac{2n}{w} & 0 & 0 & 0 \\[8pt] 0 & \dfrac{2n}{h} & 0 & 0 \\[8pt] 0 & 0 & -\dfrac{f+n}{f-n} & -\dfrac{2nf}{f-n} \\[8pt] 0 & 0 & -1 & 0 \end{bmatrix} \begin{bmatrix} x \\ y \\ z \\ 1 \end{bmatrix}$$

> **OpenGL Perspective Projection Matrix**
> - 초기 유도: $\text{near} \rightarrow +1$, $\text{far} \rightarrow -1$
> - Rasterizer 변환 후: $\text{near} \rightarrow -1$, $\text{far} \rightarrow +1$ (z에 $-1$ 적용)

### 1.6 FOV 기반 실용 공식

$w$, $h$는 near plane의 **world-space 크기**이므로, 픽셀 해상도가 아닌 FOV로 계산해야 한다.

$$h = 2n \cdot \tan\!\left(\frac{fovY}{2}\right), \quad w = h \cdot \text{aspectRatio}$$

이를 $\dfrac{2n}{h}$, $\dfrac{2n}{w}$에 대입하면:

$$\frac{2n}{h} = \frac{1}{\tan(fovY/2)}, \quad \frac{2n}{w} = \frac{1}{\text{aspectRatio} \cdot \tan(fovY/2)}$$

따라서 최종 코드:

```cpp
float tanHalfFovY = tan(fovY * 0.5f);
float aspect = (float)screenWidth / (float)screenHeight;
projMatrix[0][0] = 1.0f / (aspect * tanHalfFovY);  // 2n/w
projMatrix[1][1] = 1.0f / tanHalfFovY;              // 2n/h
```

> **주의**: `width`, `height`에 픽셀 해상도를 직접 대입하면 $2n/w \approx 0$이 되어 화면에 아무것도 보이지 않는다.

---

## 2. Orthogonal Projection Matrix

### 2.1 직육면체 (Box) 구조

원근감 없이 절두체가 직육면체 형태.

```
  y_max ─────────────
        |           |    far
  y_min ─────────────
      x_min      x_max
```

- $x \in [x_{min}, x_{max}]$, $y \in [y_{min}, y_{max}]$
- $z \in [-n, -f]$
- 모두 NDC $[-1, 1]$로 매핑, $w = 1$

### 2.2 XY 매핑

$$x_{ndc} = \frac{2}{width} \cdot x, \quad y_{ndc} = \frac{2}{height} \cdot y, \quad w = 1$$

### 2.3 Z 매핑 유도

$$z_{ndc} = az + b$$

**경계 조건:**

$$f(-n) = -1, \quad f(-f) = +1$$

연립방정식:

$$\begin{cases} -an + b = -1 \\ -af + b = +1 \end{cases}$$

두 식을 빼면:

$$a(f - n) = -2 \implies \boxed{a = -\frac{2}{f-n}}$$

대입:

$$b = -1 + an = -1 - \frac{2n}{f-n} = -\frac{f+n}{f-n} \implies \boxed{b = -\frac{f+n}{f-n}}$$

### 2.4 Orthogonal Projection Matrix

$$\begin{bmatrix} x' \\ y' \\ z' \\ w' \end{bmatrix} = \begin{bmatrix} \dfrac{2}{w} & 0 & 0 & 0 \\[8pt] 0 & \dfrac{2}{h} & 0 & 0 \\[8pt] 0 & 0 & -\dfrac{2}{f-n} & -\dfrac{f+n}{f-n} \\[8pt] 0 & 0 & 0 & 1 \end{bmatrix} \begin{bmatrix} x \\ y \\ z \\ 1 \end{bmatrix}$$

### 2.5 Perspective와의 전환 (Near Plane 기준)

Perspective ↔ Orthographic 전환 시 **near plane(화면 경계)의 크기를 기준**으로 맞추면 자연스럽게 이어진다.

$$W_{ortho} = 2n \cdot \tan\!\left(\frac{fovY}{2}\right) \cdot \text{aspectRatio}, \quad H_{ortho} = 2n \cdot \tan\!\left(\frac{fovY}{2}\right)$$

| 위치 | Perspective | Orthographic |
|------|-------------|--------------|
| Near plane ($z = -n$) | 동일 크기 | 동일 크기 |
| $z < -n$ (더 멀리) | 원근감으로 작아짐 | 그대로 (고정 스케일) |

> Near plane 기준으로 맞추므로 target distance 없이도 전환이 가능하다.
> 단, near plane 너머의 물체는 Perspective보다 Orthographic에서 크게 보이는 것이 의도된 동작이다.
