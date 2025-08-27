## Ground Rules
- Be kind, concise, and async-friendly in commits/PRs/issues.
- No pushes to `main`. All work flows through branches → PRs → reviews.
- Never commit secrets (API keys, .env). Use `.env.local` and `.gitignore`.

---

## Repo Setup & Access
- **Default branch:** `main` (stable), **integration branch:** `dev`
- **Branch protection:** `main` requires PR + 1 review + passing checks
- **Access:**
  - Team members: _Write_
  - Instructor/TAs: _Read_
- See **Settings → Collaborators & teams** if you need to adjust.

---

## Branching Strategy
- Base your work off **`dev`**.
- Use short, kebab-case names:

| Type     | Pattern                         | Example                     |
|----------|----------------------------------|-----------------------------|
| Feature  | `feature/<scope>-<summary>`      | `feature/auth-login`        |
| Bugfix   | `bugfix/<scope>-<summary>`       | `bugfix/feed-scroll`        |
| Chore    | `chore/<scope>-<summary>`        | `chore/ci-lint`             |
| Docs     | `docs/<scope>-<summary>`         | `docs/api-endpoints`        |
| Experiment (short-lived) | `exp/<summary>`  | `exp/vector-search-poc`     |

---

## Commit Messages (Conventional Commits lite)
Format: `type(scope): summary`

Common types: `feat`, `fix`, `docs`, `chore`, `refactor`, `test`, `perf`

Examples:
- `feat(auth): add email/password login`
- `fix(feed): correct scroll restore on navigation`
- `docs(readme): add setup instructions`

---

## Pull Requests
1. Update from `dev`: `git fetch && git rebase origin/dev` (or merge).
2. Push your branch and open a PR **into `dev`**.
3. Fill the PR template and request a reviewer.
4. At least **1 approval** required; all checks must pass.
5. **Squash & merge** (keeps history clean).

### PR Title
Match commit style: `feat(scope): concise summary`

### PR Checklist
- [ ] Linked issue(s) (e.g., `Closes #123`)
- [ ] Scope: one focused change
- [ ] Tests added/updated (if applicable)
- [ ] Lint/type checks pass locally
- [ ] No secrets committed
- [ ] Screenshots/video for UI changes
- [ ] Docs updated (README/`/docs`/comments)

---

## Testing


---

## Running the Project Locally


---

## Issues & Labels
- Create an issue for any non-trivial work.
- Labels we use: `feature`, `bug`, `design`, `docs`, `good-first-issue`, `blocked`, `high-priority`.

**Issue Template**
- **Summary:** what/why
- **Acceptance Criteria:** bullet list of verifiable outcomes
- **Screens/Logs:** if UI or errors
- **Dependencies:** PRs, external APIs, designs

---

## Definition of Done (DoD)
A task is **Done** when:
- Code merged to `dev`
- Tests pass; coverage not reduced for touched areas
- Docs updated
- No TODOs or commented-out code left behind
- If UI: screenshots attached to PR

---

## Security & Secrets
- Do **not** commit `.env*`, service keys, or tokens.
- Rotate keys immediately if exposure is suspected.
- Use sample files: `.env.example` for required variables.


---

## How Instructors/TAs Access
- They have **Read** access (clone/view issues/PRs).
- Demos: create a `release/<date>` branch and link in the assignment.

---

**Last updated:** [2025-08-27] — propose changes via PR to this file.
