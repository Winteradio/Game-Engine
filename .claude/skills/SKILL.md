---
name: code-review
description: "브랜치 간 코드 변경사항을 비교하고 코드리뷰를 수행한다. 코드리뷰, code review, diff 비교, 브랜치 비교, PR 리뷰, 변경사항 검토 요청 시 반드시 사용한다."
argument-hint: "비교 대상 브랜치명 (기본값: origin/master). 예) origin/dev, origin/staging"
---

# 브랜치 코드 리뷰 스킬

커밋된 변경사항 + Staged + Unstaged 변경사항을 모두 포함해 종합적으로 리뷰한다.

## Procedure

### 1. 브랜치 확인 및 BASE_BRANCH 결정

git branch --show-current && git branch -a
git fetch origin

**BASE_BRANCH 우선순위**: ① 인자로 전달된 브랜치 → ② 메시지에 언급된 브랜치 → ③ 기본값 `origin/master`

- `origin/` 없으면 자동으로 `origin/<브랜치명>`으로 변환
- 원격 브랜치 미존재 시 로컬 브랜치 사용 후 사용자에게 안내

### 2. 변경사항 수집

# 워킹 트리 상태
git status --short

# 파일 목록
git diff --name-status BASE_BRANCH...HEAD
git diff --name-status --cached
git diff --name-status

# 상세 diff
git diff -U5 --ignore-blank-lines BASE_BRANCH...HEAD  # 커밋된 변경
git diff -U5 --ignore-blank-lines --cached            # Staged
git diff -U5 --ignore-blank-lines                     # Unstaged

# 커밋 히스토리
git log --oneline BASE_BRANCH..HEAD


> diff 500줄 초과 시 파일 단위로 분할 분석. `pnpm-lock.yaml`, `package-lock.json` 등 자동 생성 파일은 제외.

### 3. 리뷰 수행

| 관점 | 체크 항목 |
|------|-----------|
| 코드 품질 | 가독성, 중복, 명명 규칙, 불필요한 로그 |
| 보안 | 인젝션, 민감 정보 노출, 인증/인가, SSRF |
| 성능 | N+1 쿼리, 불필요한 연산, 비동기 처리 |
| 유지보수성 | 단일 책임 원칙, 의존성 방향, 테스트 가능성 |
| 타입 안전성 | TypeScript `any` 남용, 타입 단언 오용 |
| 에러 처리 | 예외 처리 누락, 에러 메시지 노출 |

### 4. 결과 출력 (한국어)


## 코드 리뷰 결과

**브랜치**: `<현재>` ← `<BASE_BRANCH>`
**변경 파일**: N개  |  **추가**: +N줄  |  **삭제**: -N줄
📌 리뷰 범위: [커밋됨 / Staged / Unstaged] (해당 범위만 표시)

---
### 커밋 요약
- <해시> <메시지>

### 워킹 트리 현황
- Staged / Unstaged / Untracked (해당 항목만 표시)

### 변경사항 요약
<파일별 1~2줄 요약>

---
### 리뷰 소견

#### 🔴 Critical (즉시 수정)
#### 🟡 Warning (개선 권장)
#### 🟢 Suggestion (선택적 개선)

---
### 종합 의견


**심각도 기준**
- 🔴 보안 취약점, 런타임 오류, 데이터 손실 위험
- 🟡 성능 저하, 유지보수성 문제, 베스트 프랙티스 위반
- 🟢 코드 스타일, 가독성, 선택적 리팩토링

## 주의사항

- diff 없으면 사용자에게 안내
- 민감 정보(API 키 등) 발견 시 🔴 Critical로 즉시 보고
- 파일 20개 이상이면 핵심 파일 중심 요약 리뷰
- Unstaged 변경사항 있으면 커밋/스테이징 여부 안내