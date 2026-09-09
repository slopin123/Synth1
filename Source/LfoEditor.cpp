#include "LfoEditor.h"

// Bezier Curve Math
static float applyExponentialTension(float t, float tension)
{
    if (std::abs(tension) < 0.001f)
        return t;

    float absTension = std::abs(tension);
    float power = std::pow(10.0f, absTension * 1.3f);

    if (tension > 0.0f)
        return std::pow(t, power);

    return 1.0f - std::pow(1.0f - t, power);
}

static juce::Point<float> evaluateBezierSegment(
    juce::Point<float> p1,
    juce::Point<float> p2,
    float tension,
    float t)
{
    float x = juce::jmap(t, p1.x, p2.x);

    if (std::abs(tension) < 0.001f || std::abs(p1.y - p2.y) < 0.0001f)
        return { x, juce::jmap(t, p1.y, p2.y) };

    float curvedT = applyExponentialTension(t, tension);
    float y = juce::jmap(curvedT, p1.y, p2.y);

    return { x, y };
}

LfoEditor::LfoEditor()
{
    setMouseCursor(juce::MouseCursor::NormalCursor);
}

void LfoEditor::setAudioPhase(float phase, bool isVoiceActive) noexcept
{
    currentAudioPhase = phase;
    active = isVoiceActive;
    repaint();
}

void LfoEditor::notifyTableUpdated()
{
    if (onTableUpdated != nullptr)
    {
        std::array<float, 512> table{};
        for (int i = 0; i < 512; ++i)
        {
            float phase = i / 511.0f;
            table[i] = getSampleAtPhase(phase);
        }
        onTableUpdated(table);
    }
}

float LfoEditor::getSampleAtPhase(float phase) const
{
    if (points.empty()) return 0.0f;
    if (phase <= points.front().pos.x) return points.front().pos.y;
    if (phase >= points.back().pos.x) return points.back().pos.y;

    for (size_t i = 0; i < points.size() - 1; ++i)
    {
        if (phase >= points[i].pos.x && phase <= points[i + 1].pos.x)
        {
            float segmentWidth = points[i + 1].pos.x - points[i].pos.x;
            if (segmentWidth < 0.0001f) return points[i].pos.y;

            float t = (phase - points[i].pos.x) / segmentWidth;
            return evaluateBezierSegment(points[i].pos, points[i + 1].pos, points[i].tension, t).y;
        }
    }

    return 0.0f;
}

void LfoEditor::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();

    g.fillAll(juce::Colours::black);

    // Grid
    g.setColour(juce::Colours::darkgrey.withAlpha(0.5f));
    for (int i = 0; i <= 8; ++i)
    {
        float x = bounds.getX() + (i / 8.0f) * bounds.getWidth();
        g.drawVerticalLine(juce::roundToInt(x), bounds.getY(), bounds.getBottom());
    }

    for (int i = 0; i <= 4; ++i)
    {
        float y = bounds.getY() + (i / 4.0f) * bounds.getHeight();
        g.drawHorizontalLine(juce::roundToInt(y), bounds.getX(), bounds.getRight());
    }

    // Curve rendering
    juce::Path path;

    if (!points.empty())
    {
        path.startNewSubPath(pointToScreen(points[0].pos));

        for (size_t i = 0; i < points.size() - 1; ++i)
        {
            auto p1 = points[i].pos;
            auto p2 = points[i + 1].pos;
            float tension = points[i].tension;

            constexpr int subdivisions = 32;
            for (int s = 1; s <= subdivisions; ++s)
            {
                float t = s / static_cast<float>(subdivisions);
                juce::Point<float> curvePt = evaluateBezierSegment(p1, p2, tension, t);
                path.lineTo(pointToScreen(curvePt));
            }
        }
    }

    g.setColour(juce::Colours::white);
    g.strokePath(path, juce::PathStrokeType(1.5f));

    // Render Playhead (Synced directly from audio phase)
    if (active)
    {
        float currentValue = getSampleAtPhase(currentAudioPhase);
        auto playheadScreenPos = pointToScreen({ currentAudioPhase, currentValue });

        // Vertical playhead line
        g.setColour(juce::Colours::orange.withAlpha(0.8f));
        g.drawVerticalLine(juce::roundToInt(playheadScreenPos.x), bounds.getY(), bounds.getBottom());

        // Intersecting ball on the curve
        g.setColour(juce::Colours::orange);
        g.fillEllipse(playheadScreenPos.x - 5.0f, playheadScreenPos.y - 5.0f, 10.0f, 10.0f);
    }

    // Tension handles
    for (size_t i = 0; i < points.size() - 1; ++i)
    {
        auto handlePos = getTensionHandlePosition(static_cast<int>(i));
        float radius = (static_cast<int>(i) == selectedTension) ? 5.0f : 3.5f;

        g.setColour(juce::Colours::cyan);
        g.fillEllipse(handlePos.x - radius, handlePos.y - radius, radius * 2.0f, radius * 2.0f);
    }

    // Nodes
    for (size_t i = 0; i < points.size(); ++i)
    {
        auto position = pointToScreen(points[i].pos);
        float radius = (static_cast<int>(i) == selectedPoint) ? 7.0f : 5.0f;

        g.setColour(juce::Colours::white);
        g.fillEllipse(position.x - radius, position.y - radius, radius * 2.0f, radius * 2.0f);
    }
}

void LfoEditor::mouseDown(const juce::MouseEvent& event)
{
    selectedPoint = -1;
    selectedTension = -1;

    if (event.mods.isRightButtonDown())
    {
        int point = findPointAt(event.position);

        if (point >= 0)
        {
            if (point == 0 || point == static_cast<int>(points.size()) - 1)
                return;

            points.erase(points.begin() + point);
            notifyTableUpdated();
            repaint();
            return;
        }

        auto newPoint = screenToPoint(event.position);

        for (size_t i = 0; i < points.size() - 1; ++i)
        {
            if (newPoint.x > points[i].pos.x && newPoint.x < points[i + 1].pos.x)
            {
                points.insert(points.begin() + static_cast<ptrdiff_t>(i) + 1, { newPoint, 0.0f });
                selectedPoint = static_cast<int>(i) + 1;
                notifyTableUpdated();
                repaint();
                return;
            }
        }
        return;
    }

    int hitPoint = findPointAt(event.position);
    if (hitPoint >= 0)
    {
        selectedPoint = hitPoint;
        repaint();
        return;
    }

    int hitTension = findTensionHandleAt(event.position);
    if (hitTension >= 0)
    {
        selectedTension = hitTension;
        dragStartMouseY = event.position.y;
        dragStartTension = points[selectedTension].tension;
        repaint();
        return;
    }

    repaint();
}

void LfoEditor::mouseDrag(const juce::MouseEvent& event)
{
    if (selectedPoint >= 0)
    {
        auto newPoint = screenToPoint(event.position);

        if (selectedPoint == 0)
        {
            newPoint.x = 0.0f;
        }
        else if (selectedPoint == static_cast<int>(points.size()) - 1)
        {
            newPoint.x = 1.0f;
        }
        else
        {
            float minX = points[selectedPoint - 1].pos.x;
            float maxX = points[selectedPoint + 1].pos.x;
            newPoint.x = juce::jlimit(minX, maxX, newPoint.x);
        }

        points[selectedPoint].pos = newPoint;
        setMouseCursor(juce::MouseCursor::DraggingHandCursor);

        notifyTableUpdated();
        repaint();
        return;
    }

    if (selectedTension >= 0)
    {
        float deltaY = dragStartMouseY - event.position.y;

        auto p1 = points[selectedTension].pos;
        auto p2 = points[selectedTension + 1].pos;

        if (p1.y > p2.y)
        {
            deltaY = -deltaY;
        }

        float sensitivity = 0.005f;
        float newTension = dragStartTension + (-deltaY * sensitivity);

        points[selectedTension].tension = juce::jlimit(-0.95f, 0.95f, newTension);
        setMouseCursor(juce::MouseCursor::UpDownResizeCursor);

        notifyTableUpdated();
        repaint();
    }
}

void LfoEditor::mouseMove(const juce::MouseEvent& event)
{
    if (findTensionHandleAt(event.position) >= 0)
    {
        setMouseCursor(juce::MouseCursor::UpDownResizeCursor);
    }
    else if (findPointAt(event.position) >= 0)
    {
        setMouseCursor(juce::MouseCursor::DraggingHandCursor);
    }
    else
    {
        setMouseCursor(juce::MouseCursor::NormalCursor);
    }
}

juce::Point<float> LfoEditor::pointToScreen(juce::Point<float> point) const
{
    auto bounds = getLocalBounds().toFloat();
    float x = bounds.getX() + point.x * bounds.getWidth();
    float y = bounds.getBottom() - point.y * bounds.getHeight();
    return { x, y };
}

juce::Point<float> LfoEditor::screenToPoint(juce::Point<float> position) const
{
    auto bounds = getLocalBounds().toFloat();
    float x = (position.x - bounds.getX()) / bounds.getWidth();
    float y = 1.0f - (position.y - bounds.getY()) / bounds.getHeight();

    return {
        juce::jlimit(0.0f, 1.0f, x),
        juce::jlimit(0.0f, 1.0f, y)
    };
}

int LfoEditor::findPointAt(juce::Point<float> position) const
{
    constexpr float hitRadius = 10.0f;
    for (size_t i = 0; i < points.size(); ++i)
    {
        if (pointToScreen(points[i].pos).getDistanceFrom(position) <= hitRadius)
            return static_cast<int>(i);
    }
    return -1;
}

juce::Point<float> LfoEditor::getTensionHandlePosition(int segmentIndex) const
{
    auto p1 = points[segmentIndex].pos;
    auto p2 = points[segmentIndex + 1].pos;
    float tension = points[segmentIndex].tension;

    juce::Point<float> midPt = evaluateBezierSegment(p1, p2, tension, 0.5f);

    return pointToScreen(midPt);
}

int LfoEditor::findTensionHandleAt(juce::Point<float> position) const
{
    constexpr float hitRadius = 10.0f;
    for (size_t i = 0; i < points.size() - 1; ++i)
    {
        if (getTensionHandlePosition(static_cast<int>(i)).getDistanceFrom(position) <= hitRadius)
            return static_cast<int>(i);
    }
    return -1;
}